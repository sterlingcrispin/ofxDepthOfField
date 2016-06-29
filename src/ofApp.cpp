#include "ofApp.h"

// sterling crispin 2016
// thanks to arturo castro https://forum.openframeworks.cc/t/best-way-to-draw-many-primitives/16101
// thanks to kyle mcdonald https://forum.openframeworks.cc/t/large-point-cloud-w-depth-of-field/2930
//--------------------------------------------------------------
void ofApp::setup(){

    gui.setup();
    gui.add(pointCloudSize.set("DOF scale", 2.0, 0.001, 10.0));
    gui.add(pointCloudMinSize.set("DOF min scale", 1.0, 1.0, 100.0));
    gui.add(pointCloudOpacity.set("DOF opacity", 0.5, 0.1, 1.0));
    gui.add(pointCloudOpacityExponent.set("DOF opacity exponent", 8.8, 1, 40.0));
    gui.add(pointCloudFocalDistance.set("DOF focal dist", 250, 1, 400.0));
    
    
    //pointcloud setup
    shader.load("points");
    //Use GL_TEXTURE_2D textures.
    // note that we disable arb tex, meaning we will use normalized texture coordinates,
    // where a texture's x and y coordinate are each expressed as a normalized float.
    // this makes things slightly easier in the shader.
    ofDisableArbTex();
    
    ofLoadImage(dotTexture, "dot.png");
    vboModel.setMode(OF_PRIMITIVE_POINTS);

    ofBackground(0,0,0);
    ofEnableAntiAliasing();

    ofSetVerticalSync(true);
    
    
    // camera settings
    cam.setDistance(85);
    cam.setNearClip(0.01f);
    cam.setFarClip(10000);
    cam.setPosition(50,150,25);
    cam.lookAt(ofVec3f().zero());
    
    // put some verts in our vbo
    ofConePrimitive cone;
    cone.setResolution(100,100,100);
    
    
    for(int i = 0; i < cone.getConeMesh().getVertices().size(); i++){
        vboModel.addVertex( cone.getConeMesh().getVertex(i) );
    }
    
    for(int i = 0;  i < cone.getCapMesh().getVertices().size(); i++){
        vboModel.addVertex( cone.getCapMesh().getVertex(i) );
    }
    
    vboColor = ofColor(125,15,220);
    
    
    
    // this was scooped from another project where I needed bounding box information
    ofVec3f boundingBoxMax;
    boundingBoxMax.set(-INFINITY,-INFINITY,-INFINITY);
    ofVec3f boundingBoxMin;
    boundingBoxMin.set(INFINITY,INFINITY,INFINITY);

    float length = vboModel.getVertices().size();
    
    for(int i = 0; i < length; i++){
        for(int j = 0; j < 3; j++){
            if(vboModel.getVertices()[i][j] < boundingBoxMin[j]){
                boundingBoxMin[j] = vboModel.getVertices()[i][j];
            }
            
            if(vboModel.getVertices()[i][j] > boundingBoxMax[j]){
                boundingBoxMax[j] = vboModel.getVertices()[i][j];
            }
        }
    }
    
    maxdist = boundingBoxMin.distance(boundingBoxMax);
    

}



//--------------------------------------------------------------

void ofApp::drawVbo(ofVboMesh vboMesh,  ofColor vboColor){
    // blend modes get weird!
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    ofEnablePointSprites();
    ofDisableDepthTest();
    

    vector<float> pointSizes;
    float numParts = vboMesh.getVertices().size();
    // resize color and pointSize vectors
    pointSizes.resize( numParts);
    vboMesh.getColors().resize( numParts );
    
    // create focal plane, define it as a point and a normal
    ofVec3f focalPlaneLocation = cam.getPosition() +  cam.getLookAtDir().scale( pointCloudFocalDistance) ;
    ofVec3f focalPlaneNormal = cam.getLookAtDir().normalize();
    
    for(size_t j=0;j< numParts;j++){
        
        float color3D = ofNoise(vboMesh.getVertex(j));
        vboMesh.getColors()[j].r = (vboColor.r/255.0) * color3D;
        vboMesh.getColors()[j].g = (vboColor.g/255.0) * color3D;
        vboMesh.getColors()[j].b = (vboColor.b/255.0) * color3D;
        
        
        // for every point in the VBO, calculate its distance from the focal plane
        // by getting the vector from the focal plane to the vertex
        ofVec3f vecFromFocalPlaneToVert = focalPlaneLocation - vboMesh.getVertex(j);
        // and computing the dot product of that vector and the focal plane normal
        float dist = fabs( vecFromFocalPlaneToVert.dot(focalPlaneNormal) );
        
        // scale every point by its distance to that plane
        pointSizes[j] = dist * pointCloudSize;
        if(pointSizes[j] < pointCloudMinSize){
            // making sure they don't collapse too small
            pointSizes[j] = pointCloudMinSize;
        }
        
        // change the opacity based on distance
        float opacity = maxdist/dist * pow(pointCloudOpacity,pointCloudOpacityExponent);
       
        if(opacity > 255){
            opacity = 255;
        } else if(opacity < 0){
            opacity = 0;
        }
        
        vboMesh.getColors()[j].a = opacity;
        
    }
    
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glEnable(GL_POINT_SMOOTH);
    // make sure the vbo mesh knows what shader to use and that the shader has the latest computed
    vboMesh.getVbo().setAttributeData(shader.getAttributeLocation("pointsize"), &pointSizes[0], 1, pointSizes.size(), GL_STATIC_DRAW);
    
    // draw it!
    shader.begin();
    dotTexture.bind();
    vboMesh.draw();
    dotTexture.unbind();
    shader.end();
    
    
    ofEnableDepthTest();
    ofDisablePointSprites();
    //ofDisableBlendMode();

    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    cam.begin();
    ofEnableDepthTest();
    drawVbo(vboModel, vboColor);
    
    ofDisableDepthTest();
    cam.end();
    
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}