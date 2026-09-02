#include "../include/sceneLoader.h"

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

namespace sceneLoader
{
    namespace
    {
        // ============================================================
        // ParseString
        //
        // Busca:
        //
        // "name": "TestCube"
        // ============================================================

        bool ParseString(
            const std::string& json,
            const char* key,
            std::string& value)
        {
            std::string search =
                "\"" + std::string(key) + "\"";

            size_t pos =
                json.find(search);

            if (pos == std::string::npos)
                return false;

            pos =
                json.find(':', pos);

            if (pos == std::string::npos)
                return false;

            pos++;

            while (pos < json.size() &&
                (json[pos] == ' ' ||
                    json[pos] == '\t' ||
                    json[pos] == '\n' ||
                    json[pos] == '\r'))
            {
                pos++;
            }

            if (pos >= json.size() ||
                json[pos] != '"')
            {
                return false;
            }

            pos++;

            size_t end =
                json.find('"', pos);

            if (end == std::string::npos)
                return false;

            value =
                json.substr(
                    pos,
                    end - pos
                );

            return true;
        }


        // ============================================================
        // ParseVector3
        //
        // Busca:
        //
        // "position": [100.0, 200.0, 0.0]
        // ============================================================

        bool ParseVector3(
            const std::string& json,
            const char* key,
            bfp::Vector3D& value)
        {
            std::string search =
                "\"" + std::string(key) + "\"";

            size_t pos =
                json.find(search);

            if (pos == std::string::npos)
                return false;

            pos =
                json.find('[', pos);

            if (pos == std::string::npos)
                return false;

            float x = 0.0f;
            float y = 0.0f;
            float z = 0.0f;

            int result =
                sscanf_s(
                    json.c_str() + pos,
                    "[%f, %f, %f]",
                    &x,
                    &y,
                    &z
                );

            if (result != 3)
                return false;

            value.x = x;
            value.y = y;
            value.z = z;

            return true;
        }


        // ============================================================
        // ParseFloat
        //
        // Busca:
        //
        // "width": 200.0
        // ============================================================

        bool ParseFloat(
            const std::string& json,
            const char* key,
            float& value)
        {
            std::string search =
                "\"" + std::string(key) + "\"";

            size_t pos =
                json.find(search);

            if (pos == std::string::npos)
                return false;

            pos =
                json.find(':', pos);

            if (pos == std::string::npos)
                return false;

            pos++;

            int result =
                sscanf_s(
                    json.c_str() + pos,
                    "%f",
                    &value
                );

            return result == 1;
        }


        // ============================================================
        // ParseObjectType
        // ============================================================

        SceneObjectType ParseObjectType(
            const std::string& type)
        {
            if (type == "line")
                return SceneObjectType::Line;

            if (type == "rectangle")
                return SceneObjectType::Rectangle;

            if (type == "circle")
                return SceneObjectType::Circle;

            if (type == "cube")
                return SceneObjectType::Cube;

            // Valor por defecto
            return SceneObjectType::Line;
        }
    }


    // ================================================================
    // LoadScene
    // ================================================================

    bool LoadScene(
        const char* filename,
        Scene& scene,
        bfp::VirtualFileSystem& vfs)
    {
        if (filename == nullptr)
            return false;


        // ============================================================
        // Read file through VFS
        // ============================================================

        std::vector<char> data;

        if (!vfs.ReadFile(
            filename,
            data))
        {
            std::printf(
                "ERROR: No se pudo abrir: %s\n",
                filename
            );

            return false;
        }


        std::printf(
            "Archivo cargado: %zu bytes\n",
            data.size()
        );


        // ============================================================
        // Convert buffer into string
        // ============================================================

        std::string json(
            data.begin(),
            data.end()
        );


        std::printf(
            "JSON abierto correctamente: %s\n",
            filename
        );

        std::printf(
            "JSON size: %zu\n",
            json.size()
        );


        // ============================================================
        // Clear previous scene
        // ============================================================

        scene.objects.clear();


        // ============================================================
        // Find objects array
        // ============================================================

        size_t objectsPos =
            json.find("\"objects\"");

        if (objectsPos == std::string::npos)
        {
            std::printf(
                "ERROR: No existe el array 'objects'\n"
            );

            return false;
        }


        size_t arrayStart =
            json.find('[', objectsPos);

        if (arrayStart == std::string::npos)
        {
            std::printf(
                "ERROR: No se encontro '[' de objects\n"
            );

            return false;
        }


        std::printf(
            "objectsPos = %zu\n",
            objectsPos
        );

        std::printf(
            "arrayStart = %zu\n",
            arrayStart
        );


        // ============================================================
        // Parse objects
        // ============================================================

        size_t searchPos =
            arrayStart;


        while (true)
        {
            // --------------------------------------------------------
            // Find next object
            // --------------------------------------------------------

            size_t objectStart =
                json.find('{', searchPos);

            if (objectStart == std::string::npos)
                break;


            // --------------------------------------------------------
            // Find object end
            // --------------------------------------------------------

            size_t objectEnd =
                json.find('}', objectStart);

            if (objectEnd == std::string::npos)
            {
                std::printf(
                    "ERROR: objeto sin '}'\n"
                );

                break;
            }


            // --------------------------------------------------------
            // Extract object JSON
            // --------------------------------------------------------

            std::string objectJson =
                json.substr(
                    objectStart,
                    objectEnd - objectStart + 1
                );


            SceneObject object;


            // ========================================================
            // Name
            // ========================================================

            if (!ParseString(
                objectJson,
                "name",
                object.name))
            {
                std::printf(
                    "WARNING: objeto sin name\n"
                );

                object.name =
                    "Unnamed";
            }


            // ========================================================
            // Type
            // ========================================================

            std::string type;

            if (!ParseString(
                objectJson,
                "type",
                type))
            {
                std::printf(
                    "WARNING: objeto '%s' sin type\n",
                    object.name.c_str()
                );

                searchPos =
                    objectEnd + 1;

                continue;
            }


            object.type =
                ParseObjectType(type);


            // ========================================================
            // Transform
            // ========================================================

            bfp::Vector3D position(
                0.0f,
                0.0f,
                0.0f
            );

            bfp::Vector3D rotation(
                0.0f,
                0.0f,
                0.0f
            );

            bfp::Vector3D scale(
                1.0f,
                1.0f,
                1.0f
            );


            ParseVector3(
                objectJson,
                "position",
                position
            );

            ParseVector3(
                objectJson,
                "rotation",
                rotation
            );

            ParseVector3(
                objectJson,
                "scale",
                scale
            );


            object.transform.position =
                position;

            object.transform.rotation =
                rotation;

            object.transform.scale =
                scale;


            // ========================================================
            // Geometry
            // ========================================================

            object.width =
                0.0f;

            object.height =
                0.0f;

            object.radius =
                0.0f;


            ParseFloat(
                objectJson,
                "width",
                object.width
            );

            ParseFloat(
                objectJson,
                "height",
                object.height
            );

            ParseFloat(
                objectJson,
                "radius",
                object.radius
            );


            // ========================================================
            // Build transform matrix
            // ========================================================

            object.transform.UpdateMatrix();


            // ========================================================
            // Add object to scene
            // ========================================================

            scene.objects.push_back(
                object
            );


            std::printf(
                "Loaded object: %s | type=%s\n",
                object.name.c_str(),
                type.c_str()
            );


            // ========================================================
            // Continue with next object
            // ========================================================

            searchPos =
                objectEnd + 1;
        }


        // ============================================================
        // Final information
        // ============================================================

        std::printf(
            "Scene objects: %zu\n",
            scene.objects.size()
        );


        return true;
    }
}