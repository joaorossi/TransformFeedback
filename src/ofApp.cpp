#include "ofApp.h"

void ofApp::setup()
{
    // allocate buffers and create VBOs
    for (int i = 0; i < 2; ++i)
    {
        positionBuffers[i].allocate(NUM_POINTS * sizeof(glm::vec4), GL_DYNAMIC_COPY);
        velocityBuffers[i].allocate(NUM_POINTS * sizeof(glm::vec3), GL_DYNAMIC_COPY);
        ageBuffers[i].allocate(NUM_POINTS * sizeof(float), GL_DYNAMIC_COPY);
        initVelBuffers[i].allocate(NUM_POINTS * sizeof(glm::vec3), GL_DYNAMIC_COPY);

        vbos[i].setAttributeBuffer(0, positionBuffers[i], 4, 0);
        vbos[i].setAttributeBuffer(5, velocityBuffers[i], 3, 0);
        vbos[i].setAttributeBuffer(6, ageBuffers[i], 1, 0);
        vbos[i].setAttributeBuffer(7, initVelBuffers[i], 3, 0);
    }

    // create transform feedback and bind buffers
    glGenTransformFeedbacks(2, feedback);
    for (int i = 0; i < 2; ++i)
    {
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[i]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, positionBuffers[i].getId());
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velocityBuffers[i].getId());
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, ageBuffers[i].getId());
    }
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

    // setup transform feedback shader
    ofShader::TransformFeedbackSettings tfSettings;
    tfSettings.shaderFiles[GL_VERTEX_SHADER] = "update.vert";
    tfSettings.varyingsToCapture = { "out_position", "out_velocity", "out_age" };
    tfSettings.bindDefaults = false;
    tfSettings.bufferMode = GL_SEPARATE_ATTRIBS;
    updatePass.setup(tfSettings);

    // init cam position
    cam.setDistance(1000.f);

    // no ARB tex; load dot texture; load render shader
    ofDisableArbTex();
    dotTex.load("dot.png");
    renderPass.load("render");

    // setup some initial conditiond
    std::vector<glm::vec4> positions(NUM_POINTS);
    std::vector<glm::vec3> velocities(NUM_POINTS);
    std::vector<float> ages(NUM_POINTS);
    for (int i = 0; i < NUM_POINTS; ++i)
    {
        float theta = ofRandom(glm::two_pi<float>());
        float phi   = ofRandom(glm::two_pi<float>());

        glm::vec3 p(std::cos(theta) * std::cos(phi),
                    std::sin(theta),
                    std::cos(theta) * std::sin(phi));
        positions[i] = glm::vec4(p * RADIUS * ofRandom(0.5f, 1.f), 1.f);
        velocities[i] = p * ofRandom(20.f, 100.f);
        ages[i] = ofRandom(LIFE_TIME);
    }

    // set both buffers with same initial data, just in case...
    for (int i = 0; i < 2; ++i)
    {
        positionBuffers[i].setData(positions.size() * sizeof(glm::vec4), positions.data(), GL_DYNAMIC_COPY);
        velocityBuffers[i].setData(velocities.size() * sizeof(glm::vec3), velocities.data(), GL_DYNAMIC_COPY);
        ageBuffers[i].setData(ages.size() * sizeof(float), ages.data(), GL_DYNAMIC_COPY);
        initVelBuffers[i].setData(velocities.size() * sizeof(glm::vec3), velocities.data(), GL_DYNAMIC_COPY);
    }
}

void ofApp::update()
{
}

void ofApp::draw()
{
    // get time information
    float newElapsedTime = ofGetElapsedTimef();
    float delta = newElapsedTime - lastElapsedTime;
    lastElapsedTime = newElapsedTime;

    if (start)
    {
        // do transform feedback shader pass
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[pingPongFlag]);
        updatePass.beginTransformFeedback(GL_POINTS);
        updatePass.setUniform1f("delta", delta);
        updatePass.setUniform1f("time", newElapsedTime);
        updatePass.setUniform1f("life", LIFE_TIME);
        vbos[1 - pingPongFlag].draw(GL_POINTS, 0, NUM_POINTS);
        updatePass.endTransformFeedback();
    }

    // do render shader pass
    ofClear(0.f, 0.f, 0.f);
    ofEnableBlendMode(OF_BLENDMODE_ADD); // glowy
    ofEnablePointSprites();

    cam.begin();
    dotTex.getTexture().bind();
    renderPass.begin();
    renderPass.setUniform1f("life", LIFE_TIME);

    vbos[1 - pingPongFlag].draw(GL_POINTS, 0, NUM_POINTS);

    renderPass.end();
    dotTex.getTexture().unbind();
    cam.end();

    ofDisablePointSprites();
    ofDisableBlendMode();

    // swap buffers
    pingPongFlag = 1 - pingPongFlag;
}

