#ifndef Temperature_Meta_Parser_h
#define Temperature_Meta_Parser_h

#include "TemperatureServer.h"
#include "ServerMetaData.pb.h"

/**
 * Parser class to handle meta data requests
 */
class TemperatureMetaParser : public CustomBufferParser<ServerMetaData> {

    public:
        TemperatureMetaParser(TemperatureServer& parent): _parent(parent) {};

        ProtocolBuffer* parseBuffer(const ServerMetaData*, int clientID) const;

    private:
        TemperatureServer& _parent;
};

#endif

