#ifndef GEOMAP_H
#define GEOMAP_H
#include <core/math/rect2.h>
#include <core/math/math_funcs.h>

/*
       LATITUDE
          ^       
          |
          |
          |
----------+---------> LONGITUDE
          |
          |
          |


1 degree of latitude is always the same distance (around 111.1km).
However, 1 degree of longitude is a different distance depending on the latitude.
*/

/* SPACES:
    - Earth space: Our lovely planet represented by geographical coordinates.
    - World space: 3D Euclidean space of our Godot scene represented by Cartesian coordinates.
    - Grid space: Earth data is partitioned into a grid to allow loading just a part of it.
                    Represented by 2D Cartesian coordinates.
                    The origin corresponds to the origin of World space.
                    The grid must have odd dimensions, e.g. 3x3, 7x5, 1x9...
    NOTE:
    World = Godot scene
    World =/= Earth
*/

struct Longitude {
    double value; // in radians

    static Longitude degrees(double degrees) {
        return Longitude(degrees * Math_PI / 180.0);
    }

    static Longitude radians(double radians) {
        return Longitude(radians);
    }

    Longitude operator+(Longitude rhs) const {
        return Longitude(value + rhs.value); 
    }

    Longitude operator-(Longitude rhs) const {
        return Longitude(value - rhs.value); 
    }

    Longitude operator*(double scalar) const {
        return Longitude(value * scalar);
    }

private:
    explicit Longitude(double rad) : value(rad) { /* ASSERT (value >= 0.0 && value < 2pi)*/}
};

struct Latitude {
    double value; // in radians

    static Latitude degrees(double degrees) {
        return Latitude(degrees * Math_PI / 180.0);
    }
    
    static Latitude radians(double radians) {
        return Latitude(radians);
    }

    Latitude operator+(Latitude rhs) const {
        return Latitude(value + rhs.value); 
    }

    Latitude operator-(Latitude rhs) const {
        return Latitude(value - rhs.value); 
    }

    Latitude operator*(double scalar) const {
        return Latitude(value * scalar);
    }

private:
    explicit Latitude(double rad) : value(rad) { /* ASSERT (value <= pi/2 && value >= -pi/2)*/ }
};

struct GeoCoords {
    Longitude lon;
    Latitude lat;
    GeoCoords(Longitude lon = Longitude::radians(0), Latitude lat = Latitude::radians(0)) : lon(lon), lat(lat) {}

    GeoCoords operator+(GeoCoords rhs) const {
        return GeoCoords(lon + rhs.lon, lat + rhs.lat); 
    }

    GeoCoords operator-(GeoCoords rhs) const {
        return GeoCoords(lon - rhs.lon, lat - rhs.lat); 
    }

    GeoCoords operator*(double scalar) const {
        return GeoCoords(lon * scalar, lat * scalar);
    }
};

class GeoMap {
    // 1 unit is 1 metre.
    #define UNIT_IN_METRES 1.0
    // 1 degree of latitude is 111.139 km.
    #define LATITUDE_DEGREE_IN_METRES 111139.0
public:
    /* Uses bounds in Earth space to calculate the Earth space origin and
       grid space negative corner.*/
    GeoMap (const GeoCoords& min_bounds, const GeoCoords& max_bounds, const Vector2& grid_element_size = Vector2 (1000.0, 1000.0));
    /* Calculates the grid element size (e.g. 1000mx1000m) from grid size (e.g. 3x3 grid). */
    GeoMap (const GeoCoords& min_bounds, const GeoCoords& max_bounds, int64_t grid_size);

    /* Converts World space coordinates into the index of the corresponding
       grid element which can be used for serialisation. */
    unsigned int grid_index (Vector2 world_space);

    /* Converts Earth space coordinates into World space. */
    Vector2 geo_to_world (GeoCoords);
    
private:
    /* Converts World space coordinates into Grid space. */
    Vector2 world_to_grid (Vector2 world_space) const
    {
        return world_space / grid_element_size;
    }
    Vector2 grid_size () const {
        return grid_negative_corner.abs() * 2.0;
    }
    /* Geo coordinates of the place on Earth corresponding to (0, 0) in world space. */
    GeoCoords geo_origin;
    /* Size of each grid element, in World space. */
    const Vector2 grid_element_size;
    /*  Most negative corner in Grid space. E.g. a 3x3 grid has a 
        negative corner of (-1.5, -1.5). */
    Vector2 grid_negative_corner;
};

#endif // GEOMAP_H