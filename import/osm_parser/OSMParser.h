#ifndef OSMPARSER_H
#define OSMPARSER_H
#include "core/string/ustring.h"
#include "core/math/rect2.h"
#include "../GeoMap.h"
#include "core/io/xml_parser.h"
#include "core/templates/hash_map.h"
#include "../FileAccessMemoryResizable.h"
#include "../SGImport.h"

class OSMParser {
public:
    struct World {
        HashMap<int64_t, Dictionary> nodes;
        HashMap<int64_t, Dictionary> ways;
    };

    OSMParser(SGImport& sg_import): sg_import(sg_import) {}

    World import(const String& file_name);

private:
    SGImport& sg_import;
    struct ParserInfo {
        XMLParser parser;
        List<Dictionary> xml_stack;
        std::unique_ptr<GeoMap> geomap;
        Vector<Vector<Ref<FileAccessMemoryResizable>>> tile_bytes;
        PackedByteArray a;
        World world;
        ParserInfo() : geomap(nullptr) {}
    };
    /* Returns true if this is the deepest node (if we have to pop). */
    bool parse_xml_node(ParserInfo&);

    void parse_xml_node_end(ParserInfo&);

    // parse_xml_node helpers
    void parse_bounds(ParserInfo&);
    void parse_node(ParserInfo&, Dictionary& d);

    // parse_xml_node_end helpers

};

#endif // OSMPARSER_H