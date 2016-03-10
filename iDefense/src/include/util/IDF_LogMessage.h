#ifndef IDF_LOGMESSAGE_H
#define IDF_LOGMESSAGE_H
#include "iDFTypeConvert.h"
#include "IDF_LogMessage.pb.h"

namespace idf { 

bool convert(LogMessage lhs, CFMutableDictionaryRef& rhs) {
	return (rhs!=NULL &&
			(!lhs.has_debug_level() || conver_from_pb_to_simplecf(debug_level))&&
			(!lhs.has_max_size() || conver_from_pb_to_simplecf(max_size))
			);
}


bool convert(CFDictionaryRef lhs, LogMessage& rhs) {
	convert_from_simplecf_to_pb(debug_level, int32_t);
	convert_from_simplecf_to_pb(max_size, int32_t);
	return rhs.IsInitialized();
}


}

#endif /*IDF_LOGMESSAGE.H_h*/