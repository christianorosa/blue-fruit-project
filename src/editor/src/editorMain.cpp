#include <string>
#include <fstream>
#include <cstddef>

#include "../../engine/data/include/scene.h"


void SaveScene(const Scene& scene, const std::string& path)
{
    std::ofstream file(path);

    file << "{\n \"objects\": [\n";

    for (size_t i = 0; i < scene.objects.size(); i++)
    {
        const auto& obj = scene.objects[i];

        file << "  {\n";
        file << "    \"name\": \"" << obj.name << "\",\n";

        file << "    \"position\": ["
            << obj.transform.position.x << ","
            << obj.transform.position.y << ","
            << obj.transform.position.z << "],\n";

        file << "    \"rotation\": ["
            << obj.transform.rotation.x << ","
            << obj.transform.rotation.y << ","
            << obj.transform.rotation.z << "],\n";

        file << "    \"scale\": ["
            << obj.transform.scale.x << ","
            << obj.transform.scale.y << ","
            << obj.transform.scale.z << "]\n";

        file << "  }";

        if (i < scene.objects.size() - 1)
            file << ",";

        file << "\n";
    }

    file << " ]\n}";
}