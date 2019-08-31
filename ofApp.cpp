#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(3);
	ofEnableDepthTest();

	ofFbo fbo;
	fbo.allocate(ofGetWidth(), ofGetHeight());
	fbo.begin();
	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
	ofRotate(90);
	ofClear(0);
	ofSetColor(0);

	ofTrueTypeFont font;
	font.loadFont("fonts/Kazesawa-Bold.ttf", 390, true, true, true);
	/* https://github.com/kazesawa/kazesawa */

	font.drawString("Like", font.stringWidth("Like") * -0.5 - 30, font.stringHeight("Like") * 0.5 - 500);
	font.drawString("Like", font.stringWidth("Like") * -0.5 - 30, font.stringHeight("Like") * 0.5);
	font.drawString("Like", font.stringWidth("Like") * -0.5 - 30, font.stringHeight("Like") * 0.5 + 500);

	/*
	font.drawString("Like", font.stringWidth("Like") * -0.5, font.stringHeight("Like") * 0.5);
	*/

	fbo.end();

	ofPixels pixels;
	fbo.readToPixels(pixels);
	auto span = 25;

	ofColor pix_color;
	for (int x = 0; x < fbo.getWidth(); x += span) {

		for (int y = 0; y < fbo.getHeight(); y += span) {

			pix_color = pixels.getColor(x, y);
			if (pix_color != ofColor(0, 0)) {

				for (int z = span * -1; z <= span * 1; z += span) {

					this->location_list.push_back(glm::vec3(x - ofGetWidth() * 0.5, y - ofGetHeight() * 0.5, z));
				}
			}
		}
	}

	for (auto& location : this->location_list) {

		vector<int> next_index = vector<int>();
		int index = -1;
		for (auto& other : this->location_list) {

			index++;
			if (location == other) { continue; }

			float distance = glm::distance(location, other);
			if (distance <= span) {

				next_index.push_back(index);
			}
		}

		this->next_index_list.push_back(next_index);
	}

	for (int i = 0; i < 1800; i++) {

		this->actors.push_back(new Actor(this->location_list, this->next_index_list, this->destination_list));
	}
}


//--------------------------------------------------------------
void ofApp::update() {

	int frame_span = 15;
	int prev_index_size = 0;

	if (ofGetFrameNum() % frame_span == 0) {

		prev_index_size = this->destination_list.size();
	}

	for (auto actor : this->actors) {

		actor->update(frame_span, this->location_list, this->next_index_list, this->destination_list);
	}

	if (prev_index_size != 0) {

		this->destination_list.erase(this->destination_list.begin(), this->destination_list.begin() + prev_index_size);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(180);

	int index = 0;
	for (auto actor : this->actors) {

		if (index++ < ofGetFrameNum() * 8) {

			if (ofGetFrameNum() < 1500) {

				actor->draw();
			}
			else {

				if (index > ((int)ofGetFrameNum() - 1500) * 6)
				{
					actor->draw();
				}
			}

		}
	}

	this->cam.end();

	/*
	ostringstream os;
	os << setw(4) << setfill('0') << ofGetFrameNum() + 1;

	ofImage image;
	image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
	image.saveImage("image/img_" + os.str() + ".jpg");

	if (ofGetFrameNum() + 1 >= 1800) {

		std::exit(1);
	}
	*/
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(1920, 1080, OF_WINDOW);
	ofRunApp(new ofApp());
}