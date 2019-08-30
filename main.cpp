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
    BigWorld::Camera *bwCamera_;
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

//        GetContext()->GetSubsystem<Input>()->SetMouseGrabbed(false);
//        GetContext()->GetSubsystem<Input>()->SetMouseVisible(true);
    }

    virtual void Start()
    {
        const int CHUNK_WIDTH = 32;
        ChunkWorld *chunkWorld = new ChunkWorld(context_, CHUNK_WIDTH, 1, 1, 1, 1, 1, false);
        chunkWorld->addTerrainTexture("Textures/terrain0.jpg");
        chunkWorld->addTerrainTexture("Textures/terrain1.jpg");
        chunkWorld->addTerrainTexture("Textures/terrain2.jpg");
        chunkWorld->addTerrainTexture("Textures/terrain3.jpg");
        String model("Models/Plane.mdl");
        String material("Materials/Terrain.xml");
        chunkWorld->addUndergrowthModel(0, model, material, 0);
        for (int cx = -10; cx <= 10; cx++)
            for (int cy = -10; cy <= 10; cy++)
            {
                Corners corners;
                for (int x = 0; x < CHUNK_WIDTH; x++)
                    for (int y = 0; y < CHUNK_WIDTH; y++)
                    {
                        BigWorld::Corner corner;
                        corner.height = (x * y) % 10;
                        corner.ttypes.initRawFill(1);
                        corner.ttypes.rawFillByte(0, 1);
                        corners.Push(corner);
                    }
                IntVector2 pos(cx*100, cy*100);
                Chunk *chunk = new Chunk(chunkWorld, pos, corners);
                IntVector2 cpos(cx, cy);
                chunkWorld->addChunk(cpos, chunk);
            }
        IntVector2 pos(0, 0);
        bwCamera_ = chunkWorld->setUpCamera(pos, 0, Vector3(0, 30, 0), 0, 60, 0, 20);

        cameracontrol_ = new CameraControl(context_);
        cameracontrol_->setYaw(0);
        cameracontrol_->setPitch(60);

        Renderer *renderer = GetSubsystem<Renderer>();
        SharedPtr<Viewport> viewport(new Viewport(context_, chunkWorld->getScene(), bwCamera_->getRawCamera()));
        renderer->SetViewport(0, viewport);

        SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(MyApp, HandleUpdate));
    }

    void HandleUpdate(StringHash eventType, VariantMap &eventData)
    {
        cameracontrol_->update();
        Quaternion rotation;
        cameracontrol_->getRotation(rotation);
        bwCamera_->setRotation(rotation.YawAngle(), rotation.PitchAngle(), rotation.RollAngle());
        Vector3 pos;
        cameracontrol_->getFlyingMovement(pos);
        bwCamera_->applyAbsoluteMovement(pos);
    }
};

URHO3D_DEFINE_APPLICATION_MAIN(MyApp)
