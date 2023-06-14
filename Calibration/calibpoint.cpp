#include "calibpoint.h"

CalibPoint::CalibPoint(unsigned int uiIndex, QString dPhy, QString dRaw)
    : m_uiIndex(uiIndex),
      m_dPhy(dPhy),
      m_dRaw(dRaw)
{
}
unsigned int CalibPoint::index()
{
    return m_uiIndex;
}
QString CalibPoint::phy()
{
    return m_dPhy;
}
QString CalibPoint::raw()
{
    return m_dRaw;
}

CalibPointModel::CalibPointModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void CalibPointModel::addPoint(const CalibPoint &point)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_tPoint << point;
    endInsertRows();
}

void CalibPointModel::removePoint(int iIndex)
{
 qDebug("CalibPointModel::removePoint");
 //%%AP - 2021.10.27 - Ajout
 beginRemoveRows(QModelIndex(), rowCount(), rowCount());
 m_tPoint.removeAt(iIndex);
 endRemoveRows();

 qDebug("%d",m_tPoint.count());
 //%%AP - 2021.08.30 - Ajout
 beginRemoveRows(QModelIndex(), rowCount(), rowCount());
 endRemoveRows();
}

int CalibPointModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_tPoint.count();
}

QVariant CalibPointModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_tPoint.count())
        return QVariant();

    CalibPoint calibPoint = m_tPoint[index.row()];
    if (role == IndexRole)
        return calibPoint.index();
    else if (role == PhyRole)
        return calibPoint.phy();
    else if (role == RawRole)
        return calibPoint.raw();
    return QVariant();
}

QString CalibPointModel::dGetData(int iRow, int role) {
    //if (index.row() < 0 || index.row() >= m_tPoint.count())
    //    return QVariant();

    CalibPoint calibPoint = m_tPoint[iRow];
    //if (role == IndexRole)
       // return calibPoint.index();
    if (role == PhyRole)
        return calibPoint.phy();
    else if (role == RawRole)
        return calibPoint.raw();
    //return 0;
    return "";
}

void CalibPointModel::removeAll(void)
{
//%%AP - 2021.10.27
// m_tPoint.clear();
 qDebug("CalibPointModel::removeAll");
 uint8_t ucCpt;
 uint8_t ucSize = m_tPoint.length();
 // Suppression de tous les points
 for( ucCpt = ( uint8_t )0; ucCpt < ucSize; ucCpt++ )
  {
   //while( m_tPoint.length() > 0 )
    this->removePoint(0);
  }
 //%%AP - 2021.10.27 - Ajout
 beginRemoveRows(QModelIndex(), rowCount(), rowCount());
 endRemoveRows();
}

QHash<int, QByteArray> CalibPointModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IndexRole] = "point";
    roles[PhyRole]   = "physical";
    roles[RawRole]   = "raw";
    return roles;
}
