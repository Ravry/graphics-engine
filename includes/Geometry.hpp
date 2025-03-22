#pragma once
#include <vector>
#include <math.h>

namespace Geometry {
    struct Geom {
        std::vector<float> vertices;
        std::vector<unsigned int> indices; 
    };
    
    static Geom createCube() {
        std::vector<float> cubeVertices = {
            -1.0f,  1.0f, -1.0f,    // 0
            -1.0f, -1.0f, -1.0f,    // 1
            1.0f, -1.0f, -1.0f,     // 2
            1.0f,  1.0f, -1.0f,     // 3
            -1.0f,  1.0f,  1.0f,    // 4
            -1.0f, -1.0f,  1.0f,    // 5
            1.0f, -1.0f,  1.0f,     // 6
            1.0f,  1.0f,  1.0f      // 7
        };
        std::vector<unsigned int> cubeIndices = {
            // Front face
            0, 1, 2,
            2, 3, 0,

            // Left face
            0, 4, 5,
            5, 1, 0,

            // Right face
            3, 2, 6,
            6, 7, 3,

            // Back face
            7, 6, 5,
            5, 4, 7,

            // Top face
            0, 3, 7,
            7, 4, 0,

            // Bottom face
            1, 5, 6,
            6, 2, 1
        };

        return Geom {
            cubeVertices,
            cubeIndices
        };
    }

    static Geom quadGeom() {
        std::vector<float> vertices  {
            -1.f,    -1.f,    0.f,          0.f, 0.f,           0.f, 0.f, 1.f,
            1.f,    -1.f,    0.f,           1.f, 0.f,           0.f, 0.f, 1.f,
            1.f,     1.f,    0.f,           1.f, 1.f,           0.f, 0.f, 1.f,
            -1.f,     1.f,    0.f,          0.f, 1.f,           0.f, 0.f, 1.f
        };        
    
        std::vector<unsigned int> indices {
            0, 1, 2,
            2, 3, 0  
        };
    
        return Geom {
            vertices,
            indices
        };
    }    

    static Geom createSphere(const int latitudeBands, const int longitudeBands, const float radius)
    {
        std::vector<float> sphereVertices;
        std::vector<unsigned int> sphereIndices;

        // Generate vertices
        for (int lat = 0; lat <= latitudeBands; ++lat)
        {
            float theta = lat * M_PI / latitudeBands;
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);

            for (int lon = 0; lon <= longitudeBands; ++lon)
            {
                float phi = lon * 2.0 * M_PI / longitudeBands;
                float sinPhi = sin(phi);
                float cosPhi = cos(phi);

                float x = radius * cosPhi * sinTheta;
                float y = radius * cosTheta;  // This makes it centered at (0,0,0)
                float z = radius * sinPhi * sinTheta;

                sphereVertices.push_back(x);
                sphereVertices.push_back(y);
                sphereVertices.push_back(z);

                sphereVertices.push_back(0);
                sphereVertices.push_back(0);

                sphereVertices.push_back(x);
                sphereVertices.push_back(y);
                sphereVertices.push_back(z);
            }
        }

        // Generate indices for triangle strips (CCW)
        for (int lat = 0; lat < latitudeBands; ++lat)
        {
            for (int lon = 0; lon < longitudeBands; ++lon)
            {
                int first = (lat * (longitudeBands + 1)) + lon;
                int second = first + longitudeBands + 1;

                // Adjust the order of indices to make triangles CCW
                sphereIndices.push_back(first);        // 1st vertex
                sphereIndices.push_back(first + 1);    // 2nd vertex
                sphereIndices.push_back(second);       // 3rd vertex

                sphereIndices.push_back(first + 1);    // 1st vertex
                sphereIndices.push_back(second + 1);   // 2nd vertex
                sphereIndices.push_back(second);       // 3rd vertex
            }
        }

        return Geom {
            sphereVertices,
            sphereIndices
        };
    }

    static Geom createCylinder(const int segments, const float cylinderRadius, const float cylinderHeight) {
        const float halfHeight = cylinderHeight * 0.5f;
    
        std::vector<float> cylinderVertices;
        std::vector<unsigned int> cylinderIndices;
    
        // === TOP CAP ===
        // Center vertex
        int topCenterIndex = 0;
        cylinderVertices.push_back(0.0f);      // position x
        cylinderVertices.push_back(halfHeight); // position y
        cylinderVertices.push_back(0.0f);      // position z
    
        cylinderVertices.push_back(0.0f);      // texCoord x
        cylinderVertices.push_back(0.0f);      // texCoord y
    
        cylinderVertices.push_back(0.0f);      // normal x
        cylinderVertices.push_back(1.0f);      // normal y
        cylinderVertices.push_back(0.0f);      // normal z
    
        // Top ring vertices
        for (int i = 0; i <= segments; i++) {
            float theta = (float)i / (float)segments * 2.0f * M_PI;
            float x = cylinderRadius * cos(theta);
            float z = cylinderRadius * sin(theta);
    
            // Position
            cylinderVertices.push_back(x);
            cylinderVertices.push_back(halfHeight);
            cylinderVertices.push_back(z);
            
            // TexCoord
            cylinderVertices.push_back(0.f);
            cylinderVertices.push_back(0.f);
    
            // Normal (pointing up for cap)
            cylinderVertices.push_back(0.0f);
            cylinderVertices.push_back(1.0f);
            cylinderVertices.push_back(0.0f);
        }
    
        // === BOTTOM CAP ===
        // Center vertex
        int bottomCenterIndex = cylinderVertices.size() / 8;
        cylinderVertices.push_back(0.0f);       // position x
        cylinderVertices.push_back(-halfHeight); // position y
        cylinderVertices.push_back(0.0f);       // position z
    
        cylinderVertices.push_back(0.0f);      // texCoord x
        cylinderVertices.push_back(0.0f);      // texCoord y
    
        cylinderVertices.push_back(0.0f);       // normal x
        cylinderVertices.push_back(-1.0f);      // normal y
        cylinderVertices.push_back(0.0f);       // normal z
    
        // Bottom ring vertices
        for (int i = 0; i <= segments; i++) {
            float theta = (float)i / (float)segments * 2.0f * M_PI;
            float x = cylinderRadius * cos(theta);
            float z = cylinderRadius * sin(theta);
    
            // Position
            cylinderVertices.push_back(x);
            cylinderVertices.push_back(-halfHeight);
            cylinderVertices.push_back(z);
    
            // TexCoord
            cylinderVertices.push_back(0.f);
            cylinderVertices.push_back(0.f);
    
            // Normal (pointing down for bottom cap)
            cylinderVertices.push_back(0.0f);
            cylinderVertices.push_back(-1.0f);
            cylinderVertices.push_back(0.0f);
        }
        
        // === SIDE VERTICES ===
        int sideStartIndex = cylinderVertices.size() / 8;
        for (int i = 0; i <= segments; i++) {
            float theta = (float)i / (float)segments * 2.0f * M_PI;
            float x = cylinderRadius * cos(theta);
            float z = cylinderRadius * sin(theta);
    
            // Normal for the side (pointing outward)
            float nx = cos(theta);
            float nz = sin(theta);
    
            // Top ring vertex
            cylinderVertices.push_back(x);           // position x
            cylinderVertices.push_back(halfHeight);  // position y
            cylinderVertices.push_back(z);           // position z
    
            // TexCoord
            cylinderVertices.push_back((float)i / segments); // adjust this for proper texturing
            cylinderVertices.push_back(1.0f);             // texCoord y (top)
    
            cylinderVertices.push_back(nx);          // normal x
            cylinderVertices.push_back(0.0f);        // normal y
            cylinderVertices.push_back(nz);          // normal z
    
            // Bottom ring vertex
            cylinderVertices.push_back(x);            // position x
            cylinderVertices.push_back(-halfHeight);  // position y
            cylinderVertices.push_back(z);            // position z
    
            // TexCoord
            cylinderVertices.push_back((float)i / segments); // adjust this for proper texturing
            cylinderVertices.push_back(0.0f);             // texCoord y (bottom)
    
            cylinderVertices.push_back(nx);           // normal x
            cylinderVertices.push_back(0.0f);         // normal y
            cylinderVertices.push_back(nz);           // normal z
        }
    
        // === TOP CAP INDICES ===
        for (int i = 1; i <= segments; i++) {
            // Flipped order to reverse winding direction for correct culling
            cylinderIndices.push_back(topCenterIndex);
            cylinderIndices.push_back(i);
            cylinderIndices.push_back(i + 1);
        }
    
        // === BOTTOM CAP INDICES ===
        for (int i = 1; i <= segments; i++) {
            // Flipped order to reverse winding direction for correct culling
            cylinderIndices.push_back(bottomCenterIndex);
            cylinderIndices.push_back(bottomCenterIndex + i + 1);
            cylinderIndices.push_back(bottomCenterIndex + i);
        }
    
        // === SIDE INDICES ===
        for (int i = 0; i < segments; i++) {
            int top1 = sideStartIndex + (i * 2);
            int top2 = sideStartIndex + (i * 2) + 2;
            int bottom1 = sideStartIndex + (i * 2) + 1;
            int bottom2 = sideStartIndex + (i * 2) + 3;
    
            // Triangle 1 (flipped winding order)
            cylinderIndices.push_back(top1);
            cylinderIndices.push_back(top2);
            cylinderIndices.push_back(bottom1);
    
            // Triangle 2 (flipped winding order)
            cylinderIndices.push_back(top2);
            cylinderIndices.push_back(bottom2);
            cylinderIndices.push_back(bottom1);
        }
    
        return Geom {
            cylinderVertices,
            cylinderIndices
        };
    }
};