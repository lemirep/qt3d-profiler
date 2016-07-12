#ifndef JOBTRACEVIEW_H
#define JOBTRACEVIEW_H

#include <QQuickItem>
#include <QAbstractListModel>
#include <QAbstractProxyModel>
#include <QMutex>

class JobModel;
class QAbstractListModel;

struct ModelSlice
{
    qint64 startRange;
    qint64 endRange;
    QVector<QPersistentModelIndex> m_sourceIndices;

    bool operator == (const ModelSlice &other) const {
        // Assumes that if ranges are identical, then indices are automatically
        return startRange == other.startRange && endRange == other.endRange;
    }
};

class JobProxyModel : public QAbstractProxyModel
{
    Q_OBJECT
    // QAbstractItemModel interface
public:
    JobProxyModel();

    QModelIndex index(int row, int column, const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &child) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;

    // QAbstractProxyModel interface
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const Q_DECL_OVERRIDE;
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const Q_DECL_OVERRIDE;

    void clear();
    void addSlice(const ModelSlice &slice);
    void removeSlice(int slideIndex);

    QVector<ModelSlice> activeSlices() const { return m_activeSlices; }

protected:
    friend class JobTraceView; // to call createIndex

private:
    QVector<ModelSlice> m_activeSlices;
};

class JobTraceView : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal msecToPixelScale READ msecToPixelScale WRITE setMsecToPixelScale NOTIFY msecToPixelScaleChanged)
    Q_PROPERTY(qreal viewContentX READ viewContentX WRITE setViewContentX NOTIFY viewContentXChanged)
    Q_PROPERTY(qreal viewWidth READ viewWidth WRITE setViewWidth NOTIFY viewWidthChanged)
    Q_PROPERTY(qreal frameTotalDuration READ frameTotalDuration WRITE setFrameTotalDuration NOTIFY frameTotalDurationChanged)
    Q_PROPERTY(qreal frameStartTime READ frameStartTime WRITE setFrameStartTime NOTIFY frameStartTimeChanged)
    Q_PROPERTY(QAbstractItemModel *visibleJobsModel READ visibleJobsModel CONSTANT)
    Q_PROPERTY(QAbstractListModel *sourceModel READ sourceModel WRITE setSourceModel NOTIFY sourceModelChanged)

public:
    explicit JobTraceView(QQuickItem *parent = nullptr);
    ~JobTraceView();

    // Width/Height updated based on this
    void setMsecToPixelScale(qreal scale);
    qreal msecToPixelScale() const { return m_msecToPixelScale; }

    void setViewContentX(qreal contentX);
    qreal viewContentX() const { return m_viewContentX; }

    void setSourceModel(QAbstractListModel *model);
    QAbstractListModel *sourceModel() const;

    QAbstractItemModel *visibleJobsModel() const;

    void setViewWidth(qreal viewWidth);
    qreal viewWidth() const { return m_viewWidth; }

    void setFrameTotalDuration(qreal totalDuration);
    qreal frameTotalDuration() const { return m_frameTotalDuration; }

    void setFrameStartTime(qreal startTime);
    qreal frameStartTime() const { return m_frameStartTime; }

Q_SIGNALS:
    void msecToPixelScaleChanged();
    void viewContentXChanged();
    void viewWidthChanged();
    void sourceModelChanged();
    void frameTotalDurationChanged();
    void frameStartTimeChanged();

private:
    void updateVisibleModel();

    qreal m_msecToPixelScale;
    qreal m_viewContentX;
    qreal m_viewWidth;
    qreal m_frameTotalDuration;
    qreal m_previousViewContentX;
    qreal m_frameStartTime;
    bool m_rebuild;

    JobModel *m_sourceModel;
    QScopedPointer<JobProxyModel> m_visibleJobsModel;
    QVector<ModelSlice> m_slices;
    QMutex m_mutex;
};



#endif // JOBTRACEVIEW_H
