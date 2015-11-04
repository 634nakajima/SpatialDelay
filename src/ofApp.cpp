#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    sender.setup(HOST, PORT);
    gui.setup();
    gui.add(speed.setup("speed", 90, 0, 360));
    gui.add(height.setup("hight", 0, -100, 100));
    gui.add(radius.setup("radius", 150, 10, 500));
    gui.add(delayTime.setup("delay", 0.5, 0.1, 2.0));
    angle = 0;
    delayTime = 1.0;
    delayCount = 0;
    elapsed = 0.0;
    current = 0;
    origin.x = 0;
    origin.y = 0;
    origin.z = 0;

    //lo_server_thread_add_method(st, "/Listener", "fff", listener_handler, self);//angle, posX, posY, posZ
    //lo_server_thread_add_method(st, "/Attitude", "fff", attitude_handler, self);//angle, posX, posY, posZ
    //lo_server_thread_add_method(st, "/Object", "ifff", object_handler, self);//objectID (0~3), posX, posY, posZ
    //lo_server_thread_add_method(st, "/Play", "iiii", play_handler, self);//play:1 stop:0
    //lo_server_thread_add_method(st, "/Gain", "if", gain_handler, self);//objectID(0~3), Gain(0.0~1.0)
    //lo_server_thread_add_method(st, "/DistHB", "ffffffff", distHB_handler, self);//l1l2l3l4,r1r2r3r4
}

//--------------------------------------------------------------
void ofApp::update(){
    prevs[current].x = origin.x;
    prevs[current].y = origin.y;
    prevs[current].z = origin.z;
    current = (current+1)%500;
    
    angle += speed/ofGetFrameRate();
    origin.x = radius*sin(2*M_PI*angle/360.0);
    origin.y = radius*cos(2*M_PI*angle/360.0);
    origin.z = height;

    int pos1, pos2, pos3;
    float delayFrames = delayTime*ofGetFrameRate()*2;
    pos1 = current-delayFrames;
    while (pos1<0) pos1 += 500;
    pos2 = pos1-delayFrames;
    while (pos2<0) pos2 += 500;
    pos3 = pos2-delayFrames;
    while (pos3<0) pos3 += 500;
    
    d[0] = prevs[pos1];
    d[1] = prevs[pos2];
    d[2] = prevs[pos3];
    
    ofxOscMessage m;
    m.setAddress("/Object");
    m.addIntArg(0);
    m.addFloatArg(origin.x);
    m.addFloatArg(origin.z);
    m.addFloatArg(1000+origin.y);
    sender.sendMessage(m);
    m.clear();
    
    m.setAddress("/Object");
    m.addIntArg(1);
    m.addFloatArg(d[0].x);
    m.addFloatArg(d[0].z);
    m.addFloatArg(1000+d[0].y);
    sender.sendMessage(m);
    m.clear();
    
    m.setAddress("/Object");
    m.addIntArg(2);
    m.addFloatArg(d[1].x);
    m.addFloatArg(d[1].z);
    m.addFloatArg(1000+d[1].y);
    sender.sendMessage(m);
    m.clear();
    
    m.setAddress("/Object");
    m.addIntArg(3);
    m.addFloatArg(d[2].x);
    m.addFloatArg(d[2].z);
    m.addFloatArg(1000+d[2].y);
    sender.sendMessage(m);
    m.clear();
    
    if(delayCount) {
        elapsed += 1.0/ofGetFrameRate();
        if(elapsed >= delayTime) {
            elapsed -= delayTime;
            switch(delayCount) {
                case 1:
                    showD1 =true;
                    m.clear();
                    m.setAddress("/Play");
                    m.addIntArg(1);
                    m.addIntArg(1);
                    sender.sendMessage(m);
                    delayCount++;
                    break;
                case 2:
                    showD2 =true;
                    m.clear();
                    m.setAddress("/Play");
                    m.addIntArg(2);
                    m.addIntArg(1);
                    sender.sendMessage(m);
                    delayCount++;
                    break;
                case 3:
                    showD3 =true;
                    m.clear();
                    m.setAddress("/Play");
                    m.addIntArg(3);
                    m.addIntArg(1);
                    sender.sendMessage(m);
                    delayCount++;
                    break;
                case 4:
                    elapsed = 0.0;
                    delayCount = 0;
                    break;
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    gui.draw();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    ofCircle(origin.x, origin.y, 20);
    if(showD1) ofCircle(d[0].x, d[0].y, 20);
    if(showD2) ofCircle(d[1].x, d[1].y, 20);
    if(showD3) ofCircle(d[2].x, d[2].y, 20);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'p') {
        int pos1, pos2, pos3;
        float delayFrames = delayTime*ofGetFrameRate();
        pos1 = current-delayFrames;
        while (pos1<0) pos1 += 500;
        pos2 = pos1-delayFrames;
        while (pos2<0) pos2 += 500;
        pos3 = pos2-delayFrames;
        while (pos3<0) pos3 += 500;
        
        d[0] = prevs[pos1];
        d[1] = prevs[pos2];
        d[2] = prevs[pos3];
        showD1 = false;
        showD2 = false;
        showD3 = false;
        ofxOscMessage m;
        m.setAddress("/AllPlay");
        m.addIntArg(1);
        m.addIntArg(0);
        m.addIntArg(0);
        m.addIntArg(0);
        sender.sendMessage(m);
        

        for(int i=0;i<3;i++) {
            m.clear();
            m.setAddress("/Object");
            m.addIntArg(i+1);
            m.addFloatArg(d[i].x);
            m.addFloatArg(d[i].z);
            m.addFloatArg(1000+d[i].y);
            sender.sendMessage(m);
        }
        elapsed = 0.0;
        delayCount = 1;
        
    }else if (key == 's') {
        showD1 = false;
        showD2 = false;
        showD3 = false;
        ofxOscMessage m;
        m.setAddress("/AllPlay");
        m.addIntArg(0);
        m.addIntArg(0);
        m.addIntArg(0);
        m.addIntArg(0);
        sender.sendMessage(m);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
