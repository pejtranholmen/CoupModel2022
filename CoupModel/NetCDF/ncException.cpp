#include "../stdafx.h"
#include "ncException.h"
#include <sstream>
using namespace std;
using namespace netCDF;
using namespace netCDF::exceptions;


// Default object thrown if a netCDF exception is encountered.
NcException::NcException(const std::string& exceptionNameIn,const std::string& complaintIn,const char* fileNameIn,int lineNumberIn)
  :exceptionName(exceptionNameIn), complaint(complaintIn),fileName(fileNameIn), lineNumber(lineNumberIn)
{};

NcException::~NcException()throw() {}

const char* NcException::what() const throw()
{
  std::ostringstream oss;
  oss << lineNumber;
  std::string message(exceptionName+": "+complaint+"\nfile: "+fileName+"  line:"+oss.str());
  return message.c_str();
}


// Thrown if the specified netCDF ID does not refer to an open netCDF dataset. 
NcBadId::NcBadId(const std::string& complaint,const char* file,int line) :
  NcException("NcBadId",complaint,file,line) { }


// Thrown if too many netcdf files are open.
NcNFile::NcNFile(const std::string& complaint,const char* file,int line) :
  NcException("NcFile",complaint,file,line) { }

// Thrown if, having set NC_NOCLOBBER, the specified dataset already exists. 
NcExist::NcExist(const std::string& complaint,const char* file,int line) :
  NcException("NcExist",complaint,file,line) { }

// Thrown if not a netCDF id.
NcInvalidArg::NcInvalidArg(const std::string& complaint,const char* file,int line) :
  NcException("NcInvalidArg",complaint,file,line) { }

// Thrown if invalid argument.
NcInvalidWrite::NcInvalidWrite(const std::string& complaint,const char* file,int line) :
  NcException("NcInvalidWrite",complaint,file,line) { }

// Thrown if operation not allowed in data mode.
NcNotInDefineMode::NcNotInDefineMode(const std::string& complaint,const char* file,int line) :
  NcException("NcNotIndDefineMode",complaint,file,line) { }

// Thrown if operation not allowed in defined mode.
NcInDefineMode::NcInDefineMode(const std::string& complaint,const char* file,int line) :
  NcException("NcInDefineMode",complaint,file,line) { }

// Index exceeds dimension bound
NcInvalidCoords::NcInvalidCoords(const std::string& complaint,const char* file,int line) :
  NcException("NcInvalidCoords",complaint,file,line) { }

// Thrown if NC_MAX_DIMS is exceeded.
NcMaxDims::NcMaxDims(const std::string& complaint,const char* file,int line) :
  NcException("NcMaxDims",complaint,file,line) { }

// Thrown if std::string match to name is in use.
NcNameInUse::NcNameInUse(const std::string& complaint,const char* file,int line) :
  NcException("NcNameInUse",complaint,file,line) { }

// Thrown if attribute is not found.
NcNotAtt::NcNotAtt(const std::string& complaint,const char* file,int line) :
  NcException("NcNotAtt",complaint,file,line) { }

// Thrown if Nc_MAX_ATTRS is exceeded.
NcMaxAtts::NcMaxAtts(const std::string& complaint,const char* file,int line) :
  NcException("NcMaxAtts",complaint,file,line) { }

// Thrown if not a valid netCDF data type.
NcBadType::NcBadType(const std::string& complaint,const char* file,int line) :
  NcException("NcBadType",complaint,file,line) { }

// Thrown if an invalid dimension id or name.
NcBadDim::NcBadDim(const std::string& complaint,const char* file,int line) :
  NcException("NcBadDim",complaint,file,line) { }

// Thrown if Nc_UNLIMITED is in the wrong index.
NcUnlimPos::NcUnlimPos(const std::string& complaint,const char* file,int line) :
  NcException("NcUnlimPos",complaint,file,line) { }

// Thrown if NC_MAX_VARS is exceeded.
NcMaxVars::NcMaxVars(const std::string& complaint,const char* file,int line) :
  NcException("NcMaxVars",complaint,file,line) { }

// Thrown if variable is not found.
NcNotVar::NcNotVar(const std::string& complaint,const char* file,int line) :
  NcException("NcNotVar",complaint,file,line) { }

// Thrown if the action is prohibited on the NC_GLOBAL varid.
NcGlobal::NcGlobal(const std::string& complaint,const char* file,int line) :
  NcException("NcGlobal",complaint,file,line) { }

// Thrown if not a netCDF file.
NcNotNCF::NcNotNCF(const std::string& complaint,const char* file,int line) :
  NcException("NcNotNCF",complaint,file,line) { }

// Thrown if in FORTRAN, std::string is too short.
NcSts::NcSts(const std::string& complaint,const char* file,int line) :
  NcException("NcSts",complaint,file,line) { }

// Thrown if NC_MAX_NAME is exceeded.
NcMaxName::NcMaxName(const std::string& complaint,const char* file,int line) :
  NcException("NcMaxName",complaint,file,line) { }

// Thrown if NC_UNLIMITED size is already in use.
NcUnlimit::NcUnlimit(const std::string& complaint,const char* file,int line) :
  NcException("NcUnlimit",complaint,file,line) { }

// Thrown if nc_rec op when there are no record vars.
NcNoRecVars::NcNoRecVars(const std::string& complaint,const char* file,int line) :
  NcException("NcNoRecVars",complaint,file,line) { }

// Thrown if attempt to convert between text and numbers.
NcChar::NcChar(const std::string& complaint,const char* file,int line) :
  NcException("NcChar",complaint,file,line) { }

// Thrown if edge+start exceeds dimension bound.
NcEdge::NcEdge(const std::string& complaint,const char* file,int line) :
  NcException("NcEdge",complaint,file,line) { }

// Thrown if illegal stride.
NcStride::NcStride(const std::string& complaint,const char* file,int line) :
  NcException("NcStride",complaint,file,line) { }

// Thrown if attribute or variable name contains illegal characters.
NcBadName::NcBadName(const std::string& complaint,const char* file,int line) :
  NcException("NcBadName",complaint,file,line) { }

// Thrown if math result not representable.
NcRange::NcRange(const std::string& complaint,const char* file,int line) :
  NcException("NcRange",complaint,file,line) { }

// Thrown if memory allocation (malloc) failure.
NcNoMem::NcNoMem(const std::string& complaint,const char* file,int line) :
  NcException("NcNoMem",complaint,file,line) { }

// Thrown if one or more variable sizes violate format constraints
NcVarSize::NcVarSize(const std::string& complaint,const char* file,int line) :
  NcException("NcVarSize",complaint,file,line) { }

// Thrown if invalid dimension size.
NcDimSize::NcDimSize(const std::string& complaint,const char* file,int line) :
  NcException("NcDimSize",complaint,file,line) { }

// Thrown if file likely truncated or possibly corrupted.
NcTrunc::NcTrunc(const std::string& complaint,const char* file,int line) :
  NcException("NcTrunc",complaint,file,line) { }

// Thrown if an error was reported by the HDF5 layer.
NcHdfErr::NcHdfErr(const std::string& complaint,const char* file,int line) :
  NcException("NcHdfErr",complaint,file,line) { }

// Thrown if cannot read.
NcCantRead::NcCantRead(const std::string& complaint,const char* file,int line) :
  NcException("NcCantRead",complaint,file,line) { }

// Thrown if cannot write.
NcCantWrite::NcCantWrite(const std::string& complaint,const char* file,int line) :
  NcException("NcCantWrite",complaint,file,line) { }

// Thrown if cannot create.
NcCantCreate::NcCantCreate(const std::string& complaint,const char* file,int line) :
  NcException("NcCantCreate",complaint,file,line) { }

// Thrown if file meta.
NcFileMeta::NcFileMeta(const std::string& complaint,const char* file,int line) :
  NcException("NcFileMeta",complaint,file,line) { }

// Thrown if dim meta.
NcDimMeta::NcDimMeta(const std::string& complaint,const char* file,int line) :
  NcException("NcDimMeta",complaint,file,line) { }

// Thrown if attribute meta.
NcAttMeta::NcAttMeta(const std::string& complaint,const char* file,int line) :
  NcException("NcAttMeta",complaint,file,line) { }

// Thrown if variable meta.
NcVarMeta::NcVarMeta(const std::string& complaint,const char* file,int line) :
  NcException("NcVarMeta",complaint,file,line) { }

// Thrown if no compound.
NcNoCompound::NcNoCompound(const std::string& complaint,const char* file,int line) :
  NcException("NcNoCompound",complaint,file,line) { }

// Thrown if attribute exists.
NcAttExists::NcAttExists(const std::string& complaint,const char* file,int line) :
  NcException("NcAttExists",complaint,file,line) { }

// Thrown if attempting netcdf-4 operation on netcdf-3 file.
NcNotNc4::NcNotNc4(const std::string& complaint,const char* file,int line) :
  NcException("NcNotNc4",complaint,file,line) { }

// Thrown if attempting netcdf-4 operation on strict nc3 netcdf-4 file.
NcStrictNc3::NcStrictNc3(const std::string& complaint,const char* file,int line) :
  NcException("NcStrictNc3",complaint,file,line) { }

// Thrown if bad group id.
NcBadGroupId::NcBadGroupId(const std::string& complaint,const char* file,int line) :
  NcException("NcBadGroupId",complaint,file,line) { }

// Thrown if bad type id.
NcBadTypeId::NcBadTypeId(const std::string& complaint,const char* file,int line) :
  NcException("NcBadTypeId",complaint,file,line) { }

// Thrown if bad field id.
NcBadFieldId::NcBadFieldId(const std::string& complaint,const char* file,int line) :
  NcException("NcBadFieldId",complaint,file,line) { }

// Thrown if cannot find the field id.
NcUnknownName::NcUnknownName(const std::string& complaint,const char* file,int line) :
  NcException("NcUnknownName",complaint,file,line) { }

// Thrown if cannot find the field id.
NcEnoGrp::NcEnoGrp(const std::string& complaint,const char* file,int line) :
  NcException("NcEnoGrp",complaint,file,line) { }

// Thrown if cannot find the field id.
NcNullGrp::NcNullGrp(const std::string& complaint,const char* file,int line) :
  NcException("NcNullGrp",complaint,file,line) { }

// Thrown if cannot find the field id.
NcNullDim::NcNullDim(const std::string& complaint,const char* file,int line) :
  NcException("NcNullDim",complaint,file,line) { }

// Thrown if cannot find the field id.
NcNullType::NcNullType(const std::string& complaint,const char* file,int line) :
  NcException("NcNullType",complaint,file,line) { }

// Thrown if an operation to set the deflation, chunking, endianness, fill, compression, or checksum of a NcVar object is issued after a call to NcVar::getVar or NcVar::putVar.
NcElateDef::NcElateDef(const std::string& complaint,const char* file,int line) :
  NcException("NcElateDef",complaint,file,line) { }

