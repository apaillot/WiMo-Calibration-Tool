#ifndef CALIBPOINT_H
#define CALIBPOINT_H

#include <QAbstractListModel>
#include <QStringList>

class CalibPoint
{
public:
    CalibPoint(unsigned int uiIndex, QString dPhy, QString dRaw);

    unsigned int index();
    QString phy();
    QString raw();
private:
    unsigned int m_uiIndex;
    QString m_dPhy;
    QString m_dRaw;
};

class CalibPointModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum CalibRoles {
        IndexRole = Qt::UserRole + 1,
        PhyRole,
        RawRole
    };

    CalibPointModel(QObject *parent = nullptr);

    void addPoint(const CalibPoint &point);

    void removePoint(int);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    QString dGetData(int, int);

    void removeAll(void);

protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QList<CalibPoint> m_tPoint;

};

#endif // CALIBPOINT_H
