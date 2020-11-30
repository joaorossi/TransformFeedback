#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key) { if (key == 'c') start = true; }
    void keyReleased(int key) { }
    void mouseMoved(int x, int y ) { }
    void mouseDragged(int x, int y, int button) { }
    void mousePressed(int x, int y, int button) { }
    void mouseReleased(int x, int y, int button) { }
    void mouseEntered(int x, int y) { }
    void mouseExited(int x, int y) { }
    void windowResized(int w, int h) { }
    void dragEvent(ofDragInfo dragInfo) { }
    void gotMessage(ofMessage msg) { }

    static constexpr int   NUM_POINTS = 300000;
    static constexpr float RADIUS     = 500.f;
    static constexpr float LIFE_TIME  = 6.5f;
    static constexpr float GRAVITY    = 50.f;

private:
    ofEasyCam cam;

    ofShader renderPass;
    ofShader updatePass;

    std::array<ofBufferObject, 2> positionBuffers;
    std::array<ofBufferObject, 2> velocityBuffers;
    std::array<ofBufferObject, 2> ageBuffers;
    std::array<ofBufferObject, 2> initVelBuffers;

    std::array<ofVbo, 2> vbos;

    using TransformFeedbackBaseBindings = std::vector<ofShader::TransformFeedbackBaseBinding>;
    std::array<TransformFeedbackBaseBindings, 2> tfBufferBindings;

    ofImage dotTex;

    GLuint feedback[2];

    int pingPongFlag = 0;
    float lastElapsedTime = 0.f;

    bool start = false;
};
