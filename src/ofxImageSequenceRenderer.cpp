//
//  ofxImageSequenceRenderer.cpp
//
//  Created by Ben Snell in 2019
//

#include "ofxImageSequenceRenderer.hpp"

// --------------------------------------------------------
void ofxImageSequenceRenderer::setScale(float _scale) {
    
    s1 = abs(_scale);
    s30 = s1 * 30.0;
    s300 = s1 * 300.0;
    s3000 = s1 * 3000.0;
    
    // Set the camera values
    nearDist = 0;
    farDist = s30;
    aCamPos = glm::vec3(0, 0, s1);
    bCamPos = glm::vec3(0, 0, s1);
}

// --------------------------------------------------------
void ofxImageSequenceRenderer::setupParams() {
    
    vector<string> easingCurveNames = ofxAnimatable::getAllCurveNames();

    RUI_NEW_GROUP("Rndr- Transf");
    RUI_SHARE_ENUM_PARAM_WCN("Rndr- Ease Crv Transf", easeTransf, AnimCurve::EASE_IN_EASE_OUT, AnimCurve::CUBIC_BEZIER_CAMEL_LIKE, &easingCurveNames[0]);
    RUI_SHARE_PARAM_WCN("Rndr- Reverse Ease Crv", bReverseEaseTransf);
    
    RUI_NEW_GROUP("Rndr- Camera");
    RUI_SHARE_PARAM_WCN("Rndr- A Camera Position X", aCamPos[0], -s3000, s3000);
    RUI_SHARE_PARAM_WCN("Rndr- A Camera Position Y", aCamPos[1], -s3000, s3000);
    RUI_SHARE_PARAM_WCN("Rndr- A Camera Position Z", aCamPos[2], -s3000, s3000);
    RUI_SHARE_PARAM_WCN("Rndr- B Camera Position X", bCamPos[0], -s3000, s3000);
    RUI_SHARE_PARAM_WCN("Rndr- B Camera Position Y", bCamPos[1], -s3000, s3000);
    RUI_SHARE_PARAM_WCN("Rndr- B Camera Position Z", bCamPos[2], -s3000, s3000);
    RUI_SHARE_PARAM_WCN("Rndr- A Camera Look At X", aCamLookAtPos[0], -s300, s300);
    RUI_SHARE_PARAM_WCN("Rndr- A Camera Look At Y", aCamLookAtPos[1], -s300, s300);
    RUI_SHARE_PARAM_WCN("Rndr- A Camera Look At Z", aCamLookAtPos[2], -s300, s300);
    RUI_SHARE_PARAM_WCN("Rndr- B Camera Look At X", bCamLookAtPos[0], -s300, s300);
    RUI_SHARE_PARAM_WCN("Rndr- B Camera Look At Y", bCamLookAtPos[1], -s300, s300);
    RUI_SHARE_PARAM_WCN("Rndr- B Camera Look At Z", bCamLookAtPos[2], -s300, s300);
    RUI_SHARE_PARAM_WCN("Rndr- A FOV", aFov, 0, 180);
    RUI_SHARE_PARAM_WCN("Rndr- B FOV", bFov, 0, 180);
    RUI_SHARE_PARAM_WCN("Rndr- Near Dist", nearDist, -s30/2.0, s30/2.0);
    RUI_SHARE_PARAM_WCN("Rndr- Far Dist", farDist, 0, s3000);
    RUI_SHARE_ENUM_PARAM_WCN("Rndr- Up Vector", upVector, UpVector::UP_X, UpVector::UP_Z, getUpVectorNames());
    RUI_SHARE_PARAM_WCN("Rndr- Flip Up Vector", bFlipUpVector);
    
    RUI_NEW_GROUP("Rndr- General Rendering");
    RUI_SHARE_PARAM_WCN("Rndr- Canvas Width", canvasDims[0], 0, 7680);
    RUI_SHARE_PARAM_WCN("Rndr- Canvas Height", canvasDims[1], 0, 4320);
    
    RUI_NEW_GROUP("Rndr- Single Frame Rendering");
    RUI_SHARE_PARAM_WCN("Rndr- Render One Frame", bRenderOneFrame);
    RUI_SHARE_PARAM_WCN("Rndr- Render Frame Param", renderOneFrameParam, 0, 1);
    RUI_SHARE_PARAM_WCN("Rndr- Render 1F w/ Debug Param", bRenderOneFrameWithDebugParam);
    RUI_SHARE_PARAM_WCN("Rndr- Single Frame Folder Name", singleFrameFolderName);
    
    RUI_NEW_GROUP("Rndr- Sequence Rendering");
    RUI_SHARE_PARAM_WCN("Rndr- Wrapped", bWrapped);
    RUI_SHARE_PARAM_WCN("Rndr- Num Rotations", nRotations, 0, 10);
    RUI_SHARE_PARAM_WCN("Rndr- Rotation Offset", rotationOffset, 0, 1);
    RUI_SHARE_PARAM_WCN("Rndr- Rendering Length", renderingLength, 0, 1200);
    RUI_SHARE_PARAM_WCN("Rndr- FPS", framesPerSecond, 0, 60);
    RUI_SHARE_ENUM_PARAM_WCN("Rndr- Image Type", imageType, EXPORT_IMAGE_JPG, EXPORT_IMAGE_TIF, exportImageTypeNames);
    RUI_SHARE_PARAM_WCN("Rndr- Start Rendering Sequence", bStartRenderingSequence);
    RUI_SHARE_PARAM_WCN("Rndr- Stop Rendering Sequence", bStopRenderingSequence);
    RUI_SHARE_PARAM_WCN("Rndr- Rendering Start Frame", renderingStartFrame, 0, 72000);
    RUI_SHARE_PARAM_WCN("Rndr- Sequence Folder Name Prefix", sequenceFolderNamePrefix);
    RUI_SHARE_PARAM_WCN("Rndr- Sequence Parent Folder", sequenceParentFolderPath);
    
    RUI_NEW_GROUP("Rndr- Debug Rendering");
    RUI_SHARE_PARAM_WCN("Rndr- Dbg Rndr Param", debugRenderingParam, 0, 1);
    RUI_SHARE_PARAM_WCN("Rndr- Dbg Rndr Start Param ", debugRenderingStartParam, 0, 1);
    RUI_SHARE_PARAM_WCN("Rndr- Dbg Rndr Stop Param", debugRenderingStopParam, 0, 1);
    RUI_SHARE_PARAM_WCN("Rndr- Dbg Rndr Speed Mult", debugRenderingSpeedMult, 0, 25);
    RUI_SHARE_PARAM_WCN("Rndr- Debug Downsample Canvas Denom", debugDownsampleCanvasDenom, 0, 32);
    RUI_SHARE_PARAM_WCN("Rndr- Toggle Debug Play", bToggleDebugPlay);
    RUI_SHARE_PARAM_WCN("PCR- Dbg Rndr Manual Step", debugRenderingManualStep, 0, 0.05);

}

// --------------------------------------------------------
void ofxImageSequenceRenderer::setup() {

    // Add Listeners
    ofAddListener(ofEvents().keyPressed, this, &ofxImageSequenceRenderer::keyPressed);
    
    // Setup the data...
    
    // Allocate the fbos
    canvas.allocate((int)canvasDims[0], (int)canvasDims[1], GL_RGBA);
    canvas.begin();
    ofClear(0,0,0,255);
    canvas.end();
    pix.allocate(canvasDims[0], canvasDims[1], OF_IMAGE_COLOR_ALPHA);
    
    debugRenderDims = glm::vec2(
                                round(canvasDims[0]/debugDownsampleCanvasDenom),
                                round(canvasDims[1]/debugDownsampleCanvasDenom));
    canvasDebug.allocate((int)debugRenderDims[0], (int)debugRenderDims[1], GL_RGBA);
    
    // Setup the camera
    cam.setNearClip(nearDist);
    cam.setFarClip(farDist);
    
    // Set the initial rendering param to 0
    //debugRenderingParam = 0;
    RUI_PUSH_TO_CLIENT();
}

// --------------------------------------------------------
void ofxImageSequenceRenderer::update() {
    
    // Update camera params
    cam.setNearClip(nearDist);
    cam.setFarClip(farDist);
    
    // Update the rendering parameters
    if (bStartRenderingSequence) {
        bStartRenderingSequence = false;
        bRendering = true;
        RUI_PUSH_TO_CLIENT();
        
        // Set the rendering params
        nFrames = round(renderingLength*framesPerSecond);
        currentFrameIndex = CLAMP(renderingStartFrame, 0, nFrames);
        sequenceExportPath = sequenceParentFolderPath+"/"+sequenceFolderNamePrefix + "_" + ofGetTimestampString() + "/";
    }
    if (bStopRenderingSequence) {
        bStopRenderingSequence = false;
        bRendering = false;
        RUI_PUSH_TO_CLIENT();
    }
    
    // If we're rendering, update the index
    if (bRendering) {
        currentFrameIndex++;
        if (currentFrameIndex >= nFrames) bRendering = false;
    }
    
    // Update the debug
    if (bToggleDebugPlay) {
        bToggleDebugPlay = false;
        bDebugRendering = !bDebugRendering;
        RUI_PUSH_TO_CLIENT();
    }
    if (bDebugRendering) {
        // Increment the rendering param (with optional speed multiplier)
        float increment = (1.0 / ofGetFrameRate()) / renderingLength * debugRenderingSpeedMult;
        updateDebugRenderingParam(increment);
    }

    if (bDebugRendering) {
        // If the param is not in the range, then clamp it to the range
        debugRenderingParam = CLAMP(debugRenderingParam, debugRenderingStartParam, debugRenderingStopParam);
        // Increment the rendering param (with optional speed multiplier)
        float increment = (1.0/ofGetFrameRate())/renderingLength;
        debugRenderingParam += increment * debugRenderingSpeedMult;
        // Wrap it so it's within range if it isn't already
        debugRenderingParam = fmod(debugRenderingParam-debugRenderingStartParam, debugRenderingStopParam-debugRenderingStartParam)+debugRenderingStartParam;
        RUI_PUSH_TO_CLIENT();
    }
    
}

// --------------------------------------------------------
void ofxImageSequenceRenderer::render() {
    
    // Render Single Frame
    if (bRenderOneFrame || bRenderOneFrameWithDebugParam) {
        float thisParam = bRenderOneFrame ? renderOneFrameParam : debugRenderingParam;
        bRenderOneFrame = false;
        bRenderOneFrameWithDebugParam = false;
        RUI_PUSH_TO_CLIENT();
        
        // Render image
        _render(canvas, thisParam);
        
        // Save it
        string exportPath = singleFrameFolderName + "/img_" + ofGetTimestampString() + "." + getImageTypeString();
        canvas.readToPixels(pix);
        ofSaveImage(pix, exportPath);
    }
    
    // Render sequence
    if (bRendering) {
        
        // Determine the current normalized rendering param
        float param = float(currentFrameIndex) / float(nFrames);
        // Render the scan at the canvas size
        _render(canvas, param, 1.0);
        
        // Save this rendering
        stringstream ss;
        ss << setfill('0') << setw(5);
        ss << currentFrameIndex;
        string exportPath = sequenceExportPath + ss.str() + "." + getImageTypeString();
        canvas.readToPixels(pix);
        ofSaveImage(pix, exportPath);
        
    } else {
        
        // Render a debug view at a lower resolution and with fewer particles
        _render(canvasDebug, debugRenderingParam, debugDownsampleCanvasDenom);
        
    }

}

// --------------------------------------------------------
void ofxImageSequenceRenderer::_render(ofFbo& _fbo, float _param, float _scale) {
    
    // Calculate the params
    float paramLinear = getParamLinear(_param);
    float paramTransf = getParamTransf(_param);
    
    // Update the camera parameters
    glm::vec3 camPos = lerp(aCamPos, bCamPos, paramTransf);
    glm::vec3 camLookAtPos = lerp(aCamLookAtPos, bCamLookAtPos, paramTransf);
    float fov = ofLerp(aFov, bFov, paramTransf);
    glm::vec3 up = getUpVector();
    // Set position
    cam.setPosition(camPos);
    // Adjust the position of camera based on rotation
    float rotDeg = (_param * float(nRotations) + rotationOffset) * 360.0;
    cam.rotateAround(rotDeg, up, camLookAtPos);
    // Set remaining properties
    cam.lookAt(camLookAtPos, up);
    cam.setFov(fov);

    // Pass this to the user render function
    uRender(_fbo, cam, paramLinear, paramTransf, _scale);
}

// --------------------------------------------------------
void ofxImageSequenceRenderer::uRender(ofFbo& uFbo, ofCamera& uCam, float paramLinear, 
    float paramTransf, float scale) {

    // Render some fake input
    uFbo.begin();
    ofClear(255, 255, 255, 255);
    ofSetColor(255);
    ofPushMatrix();
    uCam.begin();
    // Scale the viewport so debug sees the same as non-debug
    ofScale(scale);
    
    ofSetColor(255, 0, 0);
    ofDrawBox(-0.5, -0.5, -0.5, 1, 1, 1);
    
    uCam.end();
    ofPopMatrix();
    uFbo.end();
}

// --------------------------------------------------------
void ofxImageSequenceRenderer::draw(float x, float y, float w, float h, bool retainAspectRatio) {

    if (retainAspectRatio) {
        (bRendering ? canvas : canvasDebug).draw(
            x, 
            y, 
            getWidth() > getHeight() ? w : getWidth() * h / getHeight(),
            getWidth() > getHeight() ? getHeight() * w / getWidth() : h
        );
    }
    else {
        (bRendering ? canvas : canvasDebug).draw(x, y, getWidth(), getHeight());
    }
}

// --------------------------------------------------------
float ofxImageSequenceRenderer::calcCurveAt(float _param, AnimCurve _curveType, bool _bReverse) {
    
    if (_bReverse) return 1.0-ofxAnimatable::calcCurveAt(1.0-_param, _curveType);
    return ofxAnimatable::calcCurveAt(_param, _curveType);
}

// --------------------------------------------------------
float ofxImageSequenceRenderer::getParamLinear(float _param) {
    
    if (bWrapped) _param = CLAMP(1.0 - abs(_param - 0.5)*2.0, 0.0, 1.0);
    return _param;
}

// --------------------------------------------------------
float ofxImageSequenceRenderer::getParamTransf(float _param) {
    
    if (bWrapped) _param = CLAMP(1.0 - abs(_param - 0.5)*2.0, 0.0, 1.0);
    return calcCurveAt(_param, easeTransf, bReverseEaseTransf);
}

// --------------------------------------------------------
string ofxImageSequenceRenderer::getImageTypeString() {
    if (imageType == EXPORT_IMAGE_INVALID || imageType == EXPORT_IMAGE_NUM)
        return exportImageTypeNames.front();
    return exportImageTypeNames[static_cast<int>(imageType)];
}

// --------------------------------------------------------
void ofxImageSequenceRenderer::updateDebugRenderingParam(float increment) {

    // If the param is not in the range, then clamp it to the range
    debugRenderingParam = CLAMP(debugRenderingParam, debugRenderingStartParam, debugRenderingStopParam);
    // Increment the rendering param (with optional speed multiplier)
    debugRenderingParam += increment + 1.0;
    // Wrap it so it's within range if it isn't already
    debugRenderingParam = fmod(debugRenderingParam - debugRenderingStartParam, debugRenderingStopParam - debugRenderingStartParam) + debugRenderingStartParam;
    RUI_PUSH_TO_CLIENT();
}

// --------------------------------------------------------
void ofxImageSequenceRenderer::keyPressed(ofKeyEventArgs& e)
{
    if (e.key == ' ')
    {
        bToggleDebugPlay = !bToggleDebugPlay;
        RUI_PUSH_TO_CLIENT();
    }
    if (e.hasModifier(OF_KEY_CONTROL)) {
        if (e.key == OF_KEY_LEFT) {
            updateDebugRenderingParam(-debugRenderingManualStep);
        }
        else if (e.key == OF_KEY_RIGHT) {
            updateDebugRenderingParam(debugRenderingManualStep);
        }
    }
}

// --------------------------------------------------------

// --------------------------------------------------------

// --------------------------------------------------------

