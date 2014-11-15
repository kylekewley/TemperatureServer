#ifndef Temperature_History_Parser_h
#define Temperature_History_Parser_h

#include <CustomBufferParser.h>

#include "TemperatureServer.h"
#include "TemperatureRequest.pb.h"

/**
 * Parser class to handle garage history requests
 */
class TemperatureHistoryParser : public CustomBufferParser<TemperatureRequest> {

    public:
        TemperatureHistoryParser(const TemperatureServer& parent): _parent(parent) {};

        ProtocolBuffer* parseBuffer(const TemperatureRequest* request, int clientID) const;

    private:
        const TemperatureServer& _parent;


};

#endif

