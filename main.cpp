#include <string>
#include <sstream>

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Skybox.h>
#include <Urho3D/Audio/Sound.h>
#include <Urho3D/Audio/SoundSource3D.h>
#include <Urho3D/Audio/SoundListener.h>
#include <Urho3D/Audio/Audio.h>
#include "chunkworld.hpp"
#include "chunk.hpp"
#include "types.hpp"
#include "cameracontrol.hpp"

using namespace Urho3D;
using namespace BigWorld;
using namespace UrhoExtras;

class MyApp: public Application
{
public:
    MyApp(Context *context)
        : Application(context)
    {
    }

    virtual void Setup()
    {
        engineParameters_["FullScreen"] = false;
        engineParameters_["WindowWidth"] = 1000;
        engineParameters_["WindowHeight"] = 700;
        engineParameters_["WindowResizable"] = false;

        GetContext()->GetSubsystem<Input>()->SetMouseGrabbed(false);
        GetContext()->GetSubsystem<Input>()->SetMouseVisible(true);
    }

    virtual void Start()
    {
        ChunkWorld *chunkWorld = new ChunkWorld(context_, 1, 1, 1, 1, 30, 30, false);
        chunkWorld->addTerrainTexture("Textures/TerrainWeights.dds");
        IntVector2 pos(0, 0);
        Corners corners;
        BigWorld::Corner corner;
        corner.height = 1;
        corner.ttypes.initRawFill(1);
        corner.ttypes.rawFillByte(0, 0);
        corners.Push(corner);
        Chunk *chunk = new Chunk(chunkWorld, pos, corners);
        chunkWorld->addChunk(pos, chunk);
        chunkWorld->setUpCamera(pos, 0, Vector3::ZERO);

        new CameraControl(context_);
    }

};

URHO3D_DEFINE_APPLICATION_MAIN(MyApp)
