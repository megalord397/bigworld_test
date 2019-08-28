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
#include "camera.hpp"

using namespace Urho3D;
using namespace BigWorld;
using namespace UrhoExtras;

class MyApp: public Application
{
public:
    CameraControl *cameracontrol_;
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
        ChunkWorld *chunkWorld = new ChunkWorld(context_, 3, 3, 1, 1, 30, 30, false);
        chunkWorld->addTerrainTexture("Textures/TerrainWeights.dds");
        String model("Models/Plane.mdl");
        String material("Materials/Terrain.xml");
        chunkWorld->addUndergrowthModel(0, model, material, 0);
        Corners corners;
        for (int x = -1; x <= 1; x++)
            for (int y = -1; y <= 1; y++)
            {
                IntVector2 pos(x, y);
                BigWorld::Corner corner;
                corner.height = 0;
                corner.ttypes.initRawFill(1);
                corner.ttypes.rawFillByte(0, 0);
                corners.Push(corner);
            }
        IntVector2 pos(0, 0);
        Chunk *chunk = new Chunk(chunkWorld, pos, corners);
        chunkWorld->addChunk(pos, chunk);
        BigWorld::Camera *camera = chunkWorld->setUpCamera(pos, 0, Vector3(0, 3, 0), 0, 20, 0, 0);

        cameracontrol_ = new CameraControl(context_);

        Renderer *renderer = GetSubsystem<Renderer>();
        SharedPtr<Viewport> viewport(new Viewport(context_, chunkWorld->getScene(), camera->getRawCamera()));
        renderer->SetViewport(0, viewport);

        SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(MyApp, HandleUpdate));
    }

    void HandleUpdate(StringHash eventType, VariantMap &eventData)
    {
        cameracontrol_->update();
    }
};

URHO3D_DEFINE_APPLICATION_MAIN(MyApp)
