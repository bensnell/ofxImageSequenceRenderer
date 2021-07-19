//
//  ofxImageSequenceRenderer.hpp
//
//  Created by Ben Snell in 2019
//

#ifndef ofxImageSequenceRenderer_hpp
#define ofxImageSequenceRenderer_hpp

#include "ofMain.h"
#include "ofxRemoteUIServer.h"
#include "ofxAnimatable.h"
#include "ofxAnimatableFloat.h"
//#include "ofxBlendFunctions.hpp"

// This class is meant to be extended for rendering any kind of objects
// from point clouds to meshes to voxel grids
class ofxImageSequenceRenderer {
public:

    // What scale are the objects of interest? This is used primarily
    // in setting the bounds for camera parameters. By default, this
    // multiplies your provided bounds by 3.
    virtual void setScale(float _scale);

    // Setup rendering parameters using ofxRemoteUI
    virtual void setupParams();

    // Setup the renderer (allocate fbos, etc.)
    virtual void setup();

    // Update the renderer
    virtual void update();

    // Render the next frame
    virtual void render();

    // Draw the last rendered frame
    // Optionally retain aspect ratio
    virtual void draw(float x, float y, float w, float h, bool retainAspectRatio = false);

    // Get the size of this renderer
    int getWidth() { return canvas.getWidth(); }
    int getHeight() { return canvas.getHeight(); }
    // Get the length of the rendering in seconds
    float getDurationSec() { return renderingLength; }
    // Get the clipping plane range
    glm::vec2 getClippingPlane() { return glm::vec2(nearDist, farDist); }
    // Get the amount debug is downsampled
    float getDebugDownsampleMultiplier() { return debugDownsampleCanvasDenom; }

    // Callbacks
    virtual void keyPressed(ofKeyEventArgs& e);

protected:

    // User-defined render function
    // Override this to define how you render.
    // Provided are two params: one which moves linearly and the other
    // moving according to the transformation curve.
    virtual void uRender(ofFbo& uFbo, ofCamera& uCam, float paramLinear,
        float paramTransf, float scale);

private:

    // -------------------------
    // Scaling Params

    float s1 = 1.0;
    float s30 = s1 * 30.0;
    float s300 = s1 * 300.0;
    float s3000 = s1 * 3000.0;

    // -------------------------
    // Source Data (to be provided by user)


    // --------------------------
    // Camera

    ofCamera cam;

    // -------------------------
    // Fbo

    // Fbo for full size rendering
    ofFbo canvas;
    ofPixels pix;

    // Intermediary Render function that prepares the user rendering
    // context.
    // Optionally pass a scale that will scale the camera. 
    void _render(ofFbo& _fbo, float _param, float _scale = 1.0);

    // --------------------------
    // Params

    // EXTANT

    // Is this rendering wrapped? That is, does it go from
    // A to B and back to A, or does it just go from A to B?
    bool bWrapped = true;

    // General Params
    float nearDist = 0.1; // must be > 0
    float farDist = 20000;

    // Easing curve for position and orientation
    AnimCurve easeTransf = AnimCurve::EASE_IN_EASE_OUT;
    bool bReverseEaseTransf = false;
    // A represents near parameters
    // B represents far parameters

    // Position of camera
    glm::vec3 aCamPos = glm::vec3(0, 0, 10);
    glm::vec3 bCamPos = glm::vec3(0, 0, 10);
    // Direction of camera
    glm::vec3 aCamLookAtPos = glm::vec3(0, 0, 0);
    glm::vec3 bCamLookAtPos = glm::vec3(0, 0, 0);
    // FOV of camera
    float aFov = 60;
    float bFov = 60;
    // Up vector of scene
    enum UpVector {
        UP_X = 0,
        UP_Y,
        UP_Z
    };
    vector<string> getUpVectorNames() { return { "X", "Y", "Z" }; }
    UpVector upVector = UpVector::UP_Z;
    bool bFlipUpVector = false;
    glm::vec3 getUpVector() {
        glm::vec3 out(0, 0, 0);
        out[CLAMP(int(upVector), 0, 2)] = bFlipUpVector ? -1 : 1;
        return out;
    }

    // General Rendering Params
    glm::vec2 canvasDims = glm::vec2(3840, 2160);

    // Single Render Params
    bool bRenderOneFrame = false;
    float renderOneFrameParam = 0.0;
    bool bRenderOneFrameWithDebugParam = false;
    string singleFrameFolderName = "singleFrames";

    // Sequence Rendering Params
    int nRotations = 2;
    float rotationOffset = 0.0; // [0,1]
    float renderingLength = 300.0; // second
    float framesPerSecond = 60;
    bool bFlipRotation = false;

    bool bStartRenderingSequence = false;
    bool bStopRenderingSequence = false;
    int renderingStartFrame = 0;
    string sequenceFolderNamePrefix = "seq";
    string sequenceParentFolderPath = "sequences";

    bool bRendering = false;
    int nFrames = 0;
    int currentFrameIndex = 0; // [0, nFrames-1]
    string sequenceExportPath;

    enum EXPORT_IMAGE_TYPE {
        EXPORT_IMAGE_INVALID = -1,
        EXPORT_IMAGE_JPG,
        EXPORT_IMAGE_PNG,
        EXPORT_IMAGE_TIF,   // best speed, while retaining best quality
        EXPORT_IMAGE_NUM
    };
    vector<string> exportImageTypeNames = { "jpg", "png", "tif" };
    EXPORT_IMAGE_TYPE imageType = EXPORT_IMAGE_TIF;
    string getImageTypeString();
    
    
    // --------------------------
    // DEBUG
    
    // Debug Rendering view
    // Set this param to what you want to manually see
    float debugRenderingParam = 0.0; // [0, 1]
    bool bPushDebugRenderingParamToRUI = true;
    
    // Adjust the bounds on the rendering window to get a close-up view
    float debugRenderingStartParam = 0.0;
    float debugRenderingStopParam = 1.0;
    
    // Speed up/down the playback
    float debugRenderingSpeedMult = 1.0;
    
    // Debug rendering eficiencies
    float debugDownsampleCanvasDenom = 4.0;
    
    // Rendering
    glm::vec2 debugRenderDims;
    ofFbo canvasDebug;
    bool bToggleDebugPlay = false;
    bool bDebugRendering = false;

    float debugRenderingManualStep = 0.01;
    
    
    // --------------------------
    // UTILS
    float calcCurveAt(float _param, AnimCurve _curveType, bool _bReverse);
    
    // Parameters
    // Wrapping is applied base on the variable bWrapped
    // linearly goes from 0 to 1 (and if wrapped, back to 0
    float getParamLinear(float _param);
    // goes from 0 to 1 along the transformation curve (and if wrapped, back to 0)
    float getParamTransf(float _param);
    
    // Lerp vec3's
    glm::vec3 lerp(glm::vec3 x, glm::vec3 y, float t) {
        return x * (1.f - t) + y * t;
    }

    // Update rendering params
    void updateDebugRenderingParam(float increment, bool bForcePushToRUI = false);
    
};

#endif /* ofxImageSequenceRenderer_hpp */
