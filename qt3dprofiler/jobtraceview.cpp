#include "jobtraceview.h"
#include "datamodels.h"
#include <QtConcurrent>

JobTraceView::JobTraceView(QQuickItem *parent)
    : QQuickItem(parent)
    , m_msecToPixelScale(0.0)
    , m_viewContentX(0.0)
    , m_viewWidth(0.0)
    , m_frameTotalDuration(0.0)
    , m_previousViewContentX(0.0)
    , m_startingDuration(0.0)
    , m_rebuild(false)
    , m_sourceModel(nullptr)
    , m_visibleJobsModel(new JobProxyModel())
{
}

JobTraceView::~JobTraceView()
{
}

void JobTraceView::setMsecToPixelScale(qreal scale)
{
    if (m_msecToPixelScale != scale) {
        m_msecToPixelScale = scale;
        emit msecToPixelScaleChanged();
        updateVisibleModel();
    }
}

void JobTraceView::setViewContentX(qreal contentX)
{
    if (m_viewContentX != contentX) {
        m_viewContentX = contentX;
        emit viewContentXChanged();
        updateVisibleModel();
    }
}

void JobTraceView::setSourceModel(QAbstractListModel *model)
{
    if (m_sourceModel != model) {
        if (m_sourceModel) {
            QObject::disconnect(m_sourceModel, &QAbstractListModel::rowsInserted, this, &JobTraceView::updateVisibleModel);
            QObject::disconnect(m_sourceModel, &QAbstractListModel::rowsRemoved, this, &JobTraceView::updateVisibleModel);
        }
        m_sourceModel = qobject_cast<JobModel *>(model);
        m_visibleJobsModel->setSourceModel(m_sourceModel);
        Q_ASSERT(m_sourceModel);
        QObject::connect(m_sourceModel, &QAbstractListModel::rowsInserted, this, &JobTraceView::updateVisibleModel);
        QObject::connect(m_sourceModel, &QAbstractListModel::rowsRemoved, this, &JobTraceView::updateVisibleModel);

        m_rebuild = true;
        updateVisibleModel();
    }
}

QAbstractListModel *JobTraceView::sourceModel() const { return m_sourceModel; }

QAbstractItemModel *JobTraceView::visibleJobsModel() const { return m_visibleJobsModel.data(); }

void JobTraceView::setViewWidth(qreal viewWidth)
{
    if (m_viewWidth != viewWidth) {
        m_viewWidth = viewWidth;
        emit viewWidthChanged();
        updateVisibleModel();
    }
}

void JobTraceView::setFrameTotalDuration(qreal totalDuration)
{
    if (m_frameTotalDuration != totalDuration) {
        m_frameTotalDuration = totalDuration;
        emit frameTotalDurationChanged();
        updateVisibleModel();
    }
}

// Takes care of removing jobs that aren't visible in the current section
void JobTraceView::updateVisibleModel()
{
    // Recompute width based on duration (m_frameTotalDuration in ns)
    const qreal maxWidth = m_frameTotalDuration * 0.000001 * m_msecToPixelScale;
    if (maxWidth != width())
        setWidth(maxWidth);

    if (m_rebuild && m_frameTotalDuration != 0.0 && m_viewWidth != 0.0 && m_msecToPixelScale != 0.0) {
        m_rebuild = false;
        m_slices.clear();
        m_slices.reserve(maxWidth / m_viewWidth);
        m_visibleJobsModel->clear();

        qreal maxStartTime = std::numeric_limits<qreal>::max();
        std::vector<Job> &jobs = m_sourceModel->items();
        for (const Job &job : jobs)
            if (job.m_frameStart < maxStartTime)
                maxStartTime = job.m_frameStart;
        for (Job &job : jobs)
            job.m_x -= maxStartTime * 0.000001;
        m_startingDuration = maxStartTime;

        auto buildSlice = [this] (ModelSlice &slice) {
            const qint64 start = slice.startRange;
            const qint64 end = slice.endRange;
            const std::vector<Job> &jobs = m_sourceModel->items();

            for (int i = 0, max = m_sourceModel->rowCount(); i < max; ++i) {
                const Job &job = jobs.at(i);
                if ((job.m_frameStart >= start && job.m_frameEnd <= end) ||         // Starts and ends withing range
                        (job.m_frameEnd >= start && job.m_frameEnd <= end) ||       // Starts anywhere but ends in the range
                        (job.m_frameStart >= start && job.m_frameStart <= end) ||   // Starts in the range but ends anywhere
                        (job.m_frameStart <= start && job.m_frameEnd >= end)) {       // Job start before and ends after range
                    QMutexLocker lock(&m_mutex);
                    slice.m_sourceIndices.push_back(m_sourceModel->index(i));
                }
            }
        };

        // Rebuild all model slices;
        qreal contentX = 0.0;
        while (contentX <= maxWidth) {
            // in ns
            const qint64 startRangeDuration = contentX / (m_msecToPixelScale * 0.000001) + m_startingDuration;
            const qint64 endRangeDuration = (contentX + m_viewWidth) / (m_msecToPixelScale * 0.000001) + m_startingDuration;

            ModelSlice slice;
            slice.startRange = startRangeDuration;
            slice.endRange = endRangeDuration;

            m_slices.push_back(slice);

            contentX += m_viewWidth;
        }
        QtConcurrent::blockingMap(m_slices, buildSlice);
    }


    // Make it so that we can see jobs for 2 * width()
    // Find jobs than range from (contentX - viewWidth * 0.5 to contentX + 1.5 * viewWidth)

    // in ns
    const qint64 startRangeDuration = std::max(m_viewContentX - m_viewWidth * 1.5, 0.0) / (m_msecToPixelScale * 0.000001) + m_startingDuration;
    const qint64 endRangeDuration = (m_viewContentX + m_viewWidth * 2.5) / (m_msecToPixelScale * 0.000001) + m_startingDuration;

    const QVector<ModelSlice> &modelActiveSlices = m_visibleJobsModel->activeSlices();
    bool containsValidSlice = false;
    // Remove slices that aren't visible
    for (int i = modelActiveSlices.size() - 1; i >= 0; --i) {
        const ModelSlice &slice = modelActiveSlices.at(i);
        if (slice.endRange < startRangeDuration ||
                slice.startRange > endRangeDuration)
            m_visibleJobsModel->removeSlice(i);
        else
            containsValidSlice = true;
    }

    // Add missing slices
    if (!containsValidSlice) {
        for (auto i = 0, c = m_slices.size(); i < c; ++i) {
            const ModelSlice &slice = m_slices.at(i);
            if (!(slice.endRange < startRangeDuration ||
                  slice.startRange > endRangeDuration))
                m_visibleJobsModel->addSlice(slice);
        }
    }
}

JobProxyModel::JobProxyModel()
{
}

QModelIndex JobProxyModel::index(int row, int , const QModelIndex &) const
{
    if (row < proxyToSourceModelIndex.size())
        return proxyToSourceModelIndex.at(row);
    qDebug() << "Bad" << row << proxyToSourceModelIndex.size();
    return QModelIndex();
}

QModelIndex JobProxyModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int JobProxyModel::rowCount(const QModelIndex &) const
{
    return proxyToSourceModelIndex.size();
}

int JobProxyModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 1;
}

// 0 -> rowCount to m_sourceModelIndex
QModelIndex JobProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    if (!proxyIndex.isValid())
        return QModelIndex();

    int idx = proxyIndex.row();
    if (idx < proxyToSourceModelIndex.size())
        return proxyToSourceModelIndex.at(idx);
    return QModelIndex();
}

// m_sourceModelIndex -> to 0 -> rowCount
QModelIndex JobProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    if (!sourceIndex.isValid())
        return QModelIndex();

    int idx = proxyToSourceModelIndex.indexOf(sourceIndex);
    if (idx != -1)
        return createIndex(idx, 0);
    return QModelIndex();
}

void JobProxyModel::clear()
{
    proxyToSourceModelIndex.clear();
}

void JobProxyModel::addSlice(const ModelSlice &slice)
{
    m_activeSlices.push_back(slice);
    layoutAboutToBeChanged();
    proxyToSourceModelIndex += slice.m_sourceIndices;
    layoutChanged();
}

void JobProxyModel::removeSlice(int sliceIndex)
{
    Q_ASSERT(m_activeSlices.size() > sliceIndex);
    const ModelSlice &slice = m_activeSlices.takeAt(sliceIndex);
    const int indicesCount = slice.m_sourceIndices.size();
    if (indicesCount > 0) {
        auto startIdx = proxyToSourceModelIndex.indexOf(slice.m_sourceIndices.first());
        if (startIdx >= 0) {
            layoutAboutToBeChanged();
            proxyToSourceModelIndex.erase(proxyToSourceModelIndex.begin() + startIdx, proxyToSourceModelIndex.begin() + startIdx + indicesCount);
            layoutChanged();
        }
    }
}
