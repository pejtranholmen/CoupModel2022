
#include "SimArchive.h"
#include "../std.h"
#ifndef COUPSTD
#include "../CoupModelDoc.h"
#endif

SimArchive::SimArchive()
{
	MySingle::instance().SetSimArchivePointer(this);
}
SimArchive::~SimArchive()
{


}


Doc *SimArchive::GetSimDocPointer(size_t index)
{
	if(index<m_SimDocArray.size())
		return (Doc*)m_SimDocArray[index].pSimDoc;//GetDocPointer(index)->GetSimDocPointer();
	else
		return nullptr;

}
#ifndef COUPSTD
CCoupModelDoc *SimArchive::GetDocPointer(size_t index)
{
	if(index<m_SimDocArray.size())
		return m_SimDocArray[index].pDoc;
	else
		return nullptr;

}
#endif
