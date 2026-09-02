#include "../include/game.h"
#include "../../graphics/graphics.h"
#include "../../data/include/sceneLoader.h"

#include <cmath>
#include <cstdio>

namespace Game
{
	Scene* scn = nullptr;

	bfp::VirtualFileSystem* vfs = nullptr;

	static double g_Time = 0.0;

	void Init()
	{
		scn = new Scene;

		vfs = new bfp::VirtualFileSystem;

		g_Time = 0.0;

		// ------------------------------------------------------------
		// Mount assets
		// ------------------------------------------------------------

		if (!vfs->MountDirectory("assets"))
		{
			std::printf(
				"ERROR: no se pudo montar assets\n"
			);
		}

		// ------------------------------------------------------------
		// Mount BFA
		// ------------------------------------------------------------

		if (!vfs->MountArchive("game.bfa"))
		{
			std::printf(
				"ERROR: no se pudo montar game.bfa\n"
			);
		}

		std::printf(
			"VFS mounts: %d\n",
			vfs->GetMountCount()
		);

		// ------------------------------------------------------------
		// Load scene
		// ------------------------------------------------------------

		if (!sceneLoader::LoadScene(
			"levels/level01.json",
			*scn,
			*vfs))
		{
			std::printf(
				"ERROR: no se pudo cargar la escena\n"
			);
		}

		std::printf(
			"Scene objects after load: %zu\n",
			scn->objects.size()
		);
	}

	void Update(double deltaTime)
	{
		g_Time += deltaTime;


		if (scn == nullptr)
			return;

		for (SceneObject& obj : scn->objects)
		{
			if (obj.type == SceneObjectType::Cube)
			{
				obj.transform.rotation.y +=
					90.0f * static_cast<float>(deltaTime);

				obj.transform.UpdateMatrix();
			}
		}

	}

	void Render()
	{
		const unsigned char r =
			static_cast<unsigned char>(
				(std::sin(g_Time) * 0.5 + 0.5) * 255.0
				);

		const unsigned char g =
			static_cast<unsigned char>(
				(std::sin(g_Time + 2.0) * 0.5 + 0.5) * 255.0
				);

		const unsigned char b =
			static_cast<unsigned char>(
				(std::sin(g_Time + 4.0) * 0.5 + 0.5) * 255.0
				);

		Graphics::Clear({ r, g, b });





		if (scn == nullptr)
			return;

		for (size_t i = 0; i < scn->objects.size(); ++i)
		{
			SceneObject& obj = scn->objects[i];

			/*std::printf(
				"Rendering object: %s\n",
				obj.name.c_str()
			);*/




			/*std::printf(
				"%s | pos=(%.2f, %.2f, %.2f) "
				"scale=(%.2f, %.2f, %.2f) "
				"width=%.2f height=%.2f radius=%.2f\n",

				obj.name.c_str(),

				obj.transform.position.x,
				obj.transform.position.y,
				obj.transform.position.z,

				obj.transform.scale.x,
				obj.transform.scale.y,
				obj.transform.scale.z,

				obj.width,
				obj.height,
				obj.radius
			);*/


			switch (obj.type)
			{
			case SceneObjectType::Line:
			{
				Graphics::DrawLine(
					static_cast<int>(obj.transform.position.x),
					static_cast<int>(obj.transform.position.y),
					static_cast<int>(obj.transform.position.x + 100.0f),
					static_cast<int>(obj.transform.position.y),
					Graphics::Color(255, 255, 255)
				);

				break;
			}


			case SceneObjectType::Rectangle:
			{
				Graphics::DrawRect(
					static_cast<int>(obj.transform.position.x),
					static_cast<int>(obj.transform.position.y),
					static_cast<int>(obj.width),
					static_cast<int>(obj.height),
					Graphics::Color(120, 40, 70)
				);

				break;
			}


			case SceneObjectType::Circle:
			{
				Graphics::DrawCircle(
					static_cast<int>(obj.transform.position.x),
					static_cast<int>(obj.transform.position.y),
					static_cast<int>(obj.radius),
					Graphics::Color(40, 120, 220)
				);

				break;
			}


			case SceneObjectType::Cube:
			{
				Graphics::DrawCube(
					obj.transform.GetMatrix(),
					Graphics::Color(255, 255, 255)
				);

				break;
			}
			}
		}
	}

	void Shutdown()
	{
		delete vfs;
		vfs = nullptr;

		delete scn;
		scn = nullptr;
	}
}