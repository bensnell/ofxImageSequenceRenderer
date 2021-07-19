# ofxImageSequenceRenderer
This is a rendering helper for OpenFrameworks that streamlines the process of exporting image sequences of 2D and 3D scenes, in preparation for video compositing. Example usage follows:

Create a class that inherits from `ofxImageSequenceRenderer`. This class will do all of your personal rendering:

*MyRenderer.hpp*

```c++
#include "ImageSequenceRenderer.hpp"

class MyRenderer : public ImageSequenceRenderer {
public:
    void setupParams();
    void setup();
    void update();
protected:
    void uRender(ofFbo* uFbo, ofCamera& uCam, float paramLinear, 
                 float paramTransf, float scale);
private:
    float myParam;
}
```

*MyRenderer.cpp*

```c++
#include "MyRenderer.hpp"

void MyRenderer::setupParams() {
    ofxImageSequenceRenderer::setupParams();
    // setup any personal parameters, for example:
    RUI_NEW_GROUP("MY_PARAMS");
    RUI_SHARE_PARAM(myParam, 0, 1);
}

void MyRenderer::setup() {
    ofImageSequenceRenderer::setup();
    // setup anything else
}

void MyRenderer::update() {
    ofImageSequenceRenderer::update();
    // update anything else
}

void MyRenderer::uRender(ofFbo* uFbo, ofCamera& uCam, float paramLinear, 
	float paramTransf, float scale) {
    // This is where all our rendering happens, for example:
    uFbo->begin();
    uCam.begin(); // don't use uCam if rendering a 2D scene
    ofSetColor(255, 0, 0);
    ofDrawBox(-0.5, -0.5, -0.5, 1, 1, 1);
    uCam.end();
    uFbo->end();
}
```

Then, utilize your class in the application, and use the RUI (ofxRemoteUI GUI) to start and stop application exporting, with the specified length, frame rate, resolution, and etc.

*ofApp.h*

```c++
#include "MyRenderer.hpp"

class ofApp : public ofBaseApp {
public:
	void setup();
    void update();
    void draw();
    MyRenderer renderer;
}
```

*ofApp.cpp*

```c++
#include "ofApp.hpp"

void ofApp::setup() {
    renderer.setup();
}

void ofApp::update() {
    renderer.update();
}

void ofApp::draw() {
    // First render imagery to fbo
    renderer.render();
    // Then, draw it for debugging purposes (if so desired)
    renderer.draw(0, 0, ofGetWidth(), ofGetHeight(), true);
}
```

