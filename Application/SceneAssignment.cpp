#include "SceneAssignment.h"
#include "GL\glew.h"

// GLM Headers
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <gtc\matrix_inverse.hpp>

//Include GLFW
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"
#include "KeyboardController.h"
#include "LoadTGA.h"

SceneAssignment::SceneAssignment()
{
}

SceneAssignment::~SceneAssignment()
{
}

void SceneAssignment::Init()
{

	// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);

	//Enable back face culling
	glEnable(GL_CULL_FACE);

	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	// Load the shader programs
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
	glUseProgram(m_programID);
	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

	m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
	m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
	m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
	m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
	m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
	m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
	m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");
	m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[2].spotDirection");
	m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[2].cosCutoff");
	m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID, "lights[2].cosInner");
	m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID, "lights[2].exponent");

	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");


	// Initialise camera properties
	camera.Init(glm::vec3(0.f, 5.f, 0.f), glm::vec3(0.f, 5.f, 10.f));

	// Init VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = nullptr;
	}

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("Axes", 10000.f, 10000.f, 10000.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("Sun", glm::vec3(1.f, 1.f, 1.f), 1.f, 16, 16);
	meshList[GEO_TEXTBOX] = MeshBuilder::GenerateQuad("TextBox", glm::vec3(1.f, 1.f, 1.f), 1.f);
	meshList[GEO_TEXTBOX]->textureID = LoadTGA("Images//TextBox.tga");

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Images//left.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Images//right.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Images//top.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Images//bottom.tga");

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Images//front.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Images//back.tga");

	meshList[GEO_WALL] = MeshBuilder::GenerateQuad("Wall", glm::vec3(1.f, 1.f, 1.f), 100.f);
	meshList[GEO_WALL]->textureID = LoadTGA("Images//brick_wall.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Images//Fangsong.tga");

	meshList[GEO_DOOR] = MeshBuilder::GenerateOBJMTL("door", "Models//door_with_frame.obj", "Models//door_with_frame.mtl");
	meshList[GEO_DOOR]->textureID = LoadTGA("Images//door.tga");

	meshList[GEO_WINDOW] = MeshBuilder::GenerateOBJMTL("window", "Models//window.obj", "Models//window.mtl");
	meshList[GEO_WINDOW]->textureID = LoadTGA("Images//window.tga");

	meshList[GEO_TABLE] = MeshBuilder::GenerateOBJMTL("table", "Models//wooden_table.obj", "Models//wooden_table.mtl");
	meshList[GEO_TABLE]->textureID = LoadTGA("Images//wooden_table.tga");

	meshList[GEO_BED] = MeshBuilder::GenerateOBJMTL("bed", "Models//bed.obj", "Models//bed.mtl");
	meshList[GEO_BED]->textureID = LoadTGA("Images//bed.tga");

	meshList[GEO_CUPBOARD] = MeshBuilder::GenerateOBJMTL("cupbaord", "Models//old_cupboard.obj", "Models//old_cupboard.mtl");
	meshList[GEO_CUPBOARD]->textureID = LoadTGA("Images//old_cupboard.tga");

	meshList[GEO_BOARD] = MeshBuilder::GenerateOBJMTL("cupbaord", "Models//board.obj", "Models//board.mtl");
	meshList[GEO_BOARD]->textureID = LoadTGA("Images//board.tga");

	meshList[GEO_LIGHT] = MeshBuilder::GenerateOBJMTL("light", "Models//light.obj", "Models//light.mtl");
	meshList[GEO_LIGHT]->textureID = LoadTGA("Images//light.tga");

	meshList[GEO_LIGHTSWITCH] = MeshBuilder::GenerateOBJMTL("light", "Models//light_switch.obj", "Models//light_switch.mtl");
	meshList[GEO_LIGHTSWITCH]->textureID = LoadTGA("Images//light_switch.tga");

	meshList[GEO_PAPER] = MeshBuilder::GenerateOBJMTL("paper", "Models//paper.obj", "Models//paper.mtl");
	meshList[GEO_PAPER]->textureID = LoadTGA("Images//paper.tga");

	meshList[GEO_PAPERMSG] = MeshBuilder::GenerateQuad("papermsg", glm::vec3(1.f, 1.f, 1.f), 1.f);
	meshList[GEO_PAPERMSG]->textureID = LoadTGA("Images//papermsg.tga");

	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	projectionStack.LoadMatrix(projection);


	glUniform1i(m_parameters[U_NUMLIGHTS], NUM_LIGHTS);

	light[0].type = Light::LIGHT_DIRECTIONAL;
	light[0].color = glm::vec3(1.f, 1.f, 1.f);
	light[0].power = 0.2f;                 // keep low for horror
	light[0].spotDirection = glm::vec3(0.f, -1.f, 0.f);
	light[0].position = glm::vec3(0.f, 1.f, 0.f);

	light[1].type = Light::LIGHT_SPOT;
	light[1].color = glm::vec3(0.7882f, 0.7804f, 0.5137f);
	light[1].position = ConvertVec3(m_player.pos);
	light[1].spotDirection = glm::vec3(0.f, 1.f, 0.f);
	light[1].power = 0.7f;
	light[1].kC = 1.0f;
	light[1].kL = 0.02f;
	light[1].kQ = 0.005f;
	light[1].cosInner = 12.5f;
	light[1].cosCutoff = 30.5f;

	light[2].type = Light::LIGHT_POINT;
	light[2].color = glm::vec3(1.f, 0.f, 0.f);
	light[2].position = glm::vec3(0.f, 10.f, 0.f);
	light[2].spotDirection = glm::vec3(0.f, 1.f, 0.f);
	light[2].power = 0.f;
	light[2].kC = 1.0f;
	light[2].kL = 0.09f;
	light[2].kQ = 0.032f;
	//light[1].position = glm::vec3(0, 50, 0);
	//light[1].color = glm::vec3(1, 1, 1);
	//light[1].type = Light::LIGHT_SPOT;
	//light[1].power = 1.f;
	//light[1].kC = 1.f;
	//light[1].kL = 0.01f;
	//light[1].kQ = 0.001f;
	//light[1].cosCutoff = 45.f;
	//light[1].cosInner = 30.f;
	//light[1].exponent = 3.f;
	//light[1].spotDirection = glm::vec3(0.f, 1.f, 0.f);

	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], cosf(glm::radians<float>(light[0].cosCutoff)));
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], cosf(glm::radians<float>(light[0].cosInner)));
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], cosf(glm::radians<float>(light[1].cosCutoff)));
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], cosf(glm::radians<float>(light[1].cosInner)));
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);

	glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &light[2].color.r);
	glUniform1i(m_parameters[U_LIGHT2_TYPE], light[2].type);
	glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
	glUniform1f(m_parameters[U_LIGHT2_KC], light[2].kC);
	glUniform1f(m_parameters[U_LIGHT2_KL], light[2].kL);
	glUniform1f(m_parameters[U_LIGHT2_KQ], light[2].kQ);
	glUniform1f(m_parameters[U_LIGHT2_COSCUTOFF], cosf(glm::radians<float>(light[2].cosCutoff)));
	glUniform1f(m_parameters[U_LIGHT2_COSINNER], cosf(glm::radians<float>(light[2].cosInner)));
	glUniform1f(m_parameters[U_LIGHT2_EXPONENT], light[2].exponent);

	enableLight = true;

	//Change this to change whole house pos
	MainBuildingOffset = Vector3(0.f, 0.f, 0.f);

	//WALLS

	//Original size is 100x100
	for (wall& wall : m_wall) {
		wall.height = 20;
		//wall.breadth = 60;

		wall.pos = Vector3(100.f, 0.f, 100.f);
	}

	m_wall[0].Scale = Vector3(0.6f, 0.2f, 0.2f);
	m_wall[0].pos = MainBuildingOffset + Vector3(0.f, 10.f, -30.f);
	m_wall[0].RotationAxis = Vector3(1.f, 0.f, 0.f);
	m_wall[0].RotationDeg = 0.f;
	m_wall[0].mass = 0.f;

	m_wall[1].Scale = Vector3(0.6f, 0.2f, 0.2f);
	m_wall[1].pos = MainBuildingOffset + Vector3(0.f, 10.f, 30.f);
	m_wall[1].RotationAxis = Vector3(1.f, 0.f, 0.f);
	m_wall[1].RotationDeg = 180.f;
	m_wall[1].mass = 0.f;

	m_wall[2].Scale = Vector3(0.6f, 0.2f, 0.2f);
	m_wall[2].pos = MainBuildingOffset + Vector3(30.f, 10.f, 0.f);
	m_wall[2].RotationAxis = Vector3(0.f, 1.f, 0.f);
	m_wall[2].RotationDeg = 270.f;
	m_wall[2].mass = 0.f;

	m_wall[3].Scale = Vector3(0.6f, 0.2f, 0.2f);
	m_wall[3].pos = MainBuildingOffset + Vector3(-30.f, 10.f, 0.f);
	m_wall[3].RotationAxis = Vector3(0.f, 1.f, 0.f);
	m_wall[3].RotationDeg = 90.f;
	m_wall[3].mass = 0.f;

	for (wall& wall : m_wall) {
		wall.angleDeg = wall.RotationDeg;
		wall.breadth = 100 * wall.Scale.x;
	}

	//WALLS

	//OBJs

	m_obj[0].Type = GEO_DOOR;
	m_obj[0].Scale = Vector3(0.08f, 0.08f, 0.08f);
	m_obj[0].pos = MainBuildingOffset + Vector3(0.f, 0.f, 29.4f);
	m_obj[0].RotationAxis = Vector3(0.f, 1.f, 0.f);
	m_obj[0].RotationDeg = 0.f;
	m_obj[0].mass = 0.f;
	m_obj[0].xLength = 8.f;
	m_obj[0].zLength = 2.f;
	m_obj[0].angleDeg = m_obj[0].RotationDeg;

	m_obj[1].Type = GEO_WINDOW;
	m_obj[1].Scale = Vector3(5.f, 5.f, 5.f);
	m_obj[1].pos = MainBuildingOffset + Vector3(15.f, 6.5f, 30.1f);
	m_obj[1].RotationAxis = Vector3(0.f, 1.f, 0.f);
	m_obj[1].RotationDeg = 180.f;
	m_obj[1].mass = 0.f;
	m_obj[1].xLength = 4.f;
	m_obj[1].Interactable = true;
	m_obj[1].angleDeg = m_obj[1].RotationDeg;

	NewObjInit(m_obj[2], GEO_WINDOW, Vector3(5.f, 5.f, 5.f), MainBuildingOffset + Vector3(-15.f, 6.5f, 30.1f), Vector3(0.f, 1.f, 0.f), 180.f, 0.f, 4.f, true);

	NewObjInit(m_obj[3], GEO_WINDOW, Vector3(5.f, 5.f, 5.f), MainBuildingOffset + Vector3(30.1f, 6.5f, 0.f), Vector3(0.f, 1.f, 0.f), -90.f, 0.f, 4.f, true);

	NewObjInit(m_obj[4], GEO_WINDOW, Vector3(5.f, 5.f, 5.f), MainBuildingOffset + Vector3(-30.1f, 6.5f, 0.f), Vector3(0.f, 1.f, 0.f), 90.f, 0.f, 4.f, true);

	NewObjInit(m_obj[5], GEO_TABLE, Vector3(7.f, 7.f, 7.f), MainBuildingOffset + Vector3(-20.f, 3.f, -20.f), Vector3(0.f, 1.f, 0.f), 90.f, 0.f, 8.f, 15.f, false);

	NewObjInit(m_obj[6], GEO_BED, Vector3(10.f, 10.f, 10.f), MainBuildingOffset + Vector3(24.f, 0.f, -20.f), Vector3(0.f, 1.f, 0.f), 0.f, 0.f, 6.f, 16.f, false);

	NewObjInit(m_obj[7], GEO_CUPBOARD, Vector3(2.f, 2.f, 2.f), MainBuildingOffset + Vector3(5.f, 0.f, -26.5f), Vector3(0.f, 1.f, 0.f), 0.f, 0.f, 10.f, 5.f, true);

	NewObjInit(m_obj[8], GEO_BOARD, Vector3(5.f, 4.f, 4.f), m_obj[0].pos + Vector3(0.f, 11.f, 0.f), Vector3(1.f, 0.f, 0.f), 270.f, 1.f, 1.f, 8.f, false);

	NewObjInit(m_obj[9], GEO_BOARD, Vector3(5.f, 4.f, 4.f), m_obj[0].pos + Vector3(0.f, 14.f, 0.f), Vector3(1.f, 0.f, 0.f), 270.f, 1.f, 1.f, 8.f, false);

	NewObjInit(m_obj[10], GEO_BOARD, Vector3(5.f, 4.f, 4.f), m_obj[0].pos + Vector3(0.f, 4.f, 0.f), Vector3(1.f, 0.f, 0.f), 270.f, 1.f, 1.f, 8.f, false);

	NewObjInit(m_obj[11], GEO_BOARD, Vector3(5.f, 4.f, 4.f), m_obj[0].pos + Vector3(0.f, 6.f, 0.f), Vector3(1.f, 0.f, 0.f), 270.f, 1.f, 1.f, 8.f, false);

	NewObjInit(m_obj[12], GEO_BOARD, Vector3(5.f, 4.f, 4.f), m_obj[0].pos + Vector3(0.f, 9.f, 0.f), Vector3(1.f, 0.f, 0.f), 270.f, 1.f, 1.f, 8.f, false);

	NewObjInit(m_obj[13], GEO_LIGHT, Vector3(0.1f, 0.1f, 0.1f), MainBuildingOffset + Vector3(0.f, 19.f, 0.f), Vector3(1.f, 0.f, 0.f), 180.f, 0.f, 0.f, 0.f, false);

	NewObjInit(m_obj[13], GEO_LIGHTSWITCH, Vector3(10.f, 10.f, 10.f), MainBuildingOffset + Vector3(-4.f, 9.f, -30.f), Vector3(0.f, 1.f, 0.f), 270.f, 0.f, 1.f, 1.f, true);

	//NewObjInit(m_obj[14], GEO_PAPER, Vector3(0.1f, 0.1f, 0.1f), Vector3(0.f, 5.f, 0.f), Vector3(0.f, 0.f, 1.f), 90.f, 0.f, 0.f, 0.f);


	//OBJs

	//PLAYER

	m_player.pos = Vector3(20, 0, -20);
	m_player.bounciness = 0.1f;
	//PLAYER

	//Others
	havingconversation = true;
	interactingmenu = false;
	ConversationTracker = 0;
	ConversationBufferBool = false;
	ObjectiveText = "";
	//Phase Opening
	ConversationText[0] = "'Urgh... w-where am I?'";
	ConversationText[1] = "'I-It's so dark...am I trapped?'";
	ConversationText[2] = "Objective: Find Light";
	ConversationIntro = false;
	ConversationText[3] = "W-What!? But..that's my handwriting!";
	ConversationText[4] = "Don't on the light huh...";
	ConversationText[5] = "Objective: Look around";
	ConversationPaper = false;
	ConversationText[6] = "O-oh gods! W-What is that Thhing!!?";
	ConversationText[7] = "Objective: HIDE";
	ConversationWindow = false;
	ConversationText[8] = "*BAM!!!*";
	ConversationText[9] = "LeT mE IN!!!!!";
	ConversationText[10] = "*BAM!!!*";
	ConversationText[11] = "i KnOW YOu'Re In tHeRE!!";
	ConversationText[12] = "*BAM!!!*";
	ConversationText[13] = "AAAAAAAAAARRRRRRUUUUHHHHHHHH!!!!";

	ConversationText[14] = "I can't see anything...";
	ConversationText[15] = "T-There's SOMETHING out there!";

	ConversationText[16] = "The End!";

	flashlightobtained = false;
	flashlighton = false;

	FinaleActivated = false;
}

void SceneAssignment::Update(double dt)
{
	HandleKeyPress();
	HandleMouseInput();

	glm::vec3 camView = glm::normalize(camera.target - camera.position);
	glm::vec3 camRight = glm::normalize(glm::cross(camView, glm::vec3(0, 1, 0)));
	if (!havingconversation && !interactingmenu) {
		if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_W))
		{
			Vector3 force(camView.x, 0, camView.z);
			m_player.AddForce(force * 100.f);
		}

		if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_S))
		{
			Vector3 force(-camView.x, 0, -camView.z);
			m_player.AddForce(force * 100.f);
		}

		if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_A))
		{
			Vector3 force(-camRight.x, 0, -camRight.z);
			m_player.AddForce(force * 100.f);
		}

		if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_D))
		{
			Vector3 force(camRight.x, 0, camRight.z);
			m_player.AddForce(force * 100.f);
		}
	}
	if (havingconversation) {
		if (!ConversationIntro && ConversationTracker == 2) {
			if (MouseController::GetInstance()->IsButtonDown(GLFW_MOUSE_BUTTON_LEFT) && !ConversationBufferBool) {
				ConversationBufferBool = true;
				havingconversation = false;
				ConversationIntro = true;
				ObjectiveText = ConversationText[ConversationTracker];
			}
		}
		if (!ConversationPaper && ConversationTracker == 5) {
			if (MouseController::GetInstance()->IsButtonDown(GLFW_MOUSE_BUTTON_LEFT) && !ConversationBufferBool) {
				ConversationBufferBool = true;
				havingconversation = false;
				ConversationPaper = true;
				flashlightobtained = true;
				ObjectiveText = ConversationText[ConversationTracker];
			}
		}
		if (!ConversationWindow && ConversationTracker == 7) {
			if (MouseController::GetInstance()->IsButtonDown(GLFW_MOUSE_BUTTON_LEFT) && !ConversationBufferBool) {
				ConversationBufferBool = true;
				havingconversation = false;
				ConversationWindow = true;
				ObjectiveText = ConversationText[ConversationTracker];
				light[2].power = 2.f;
				glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
			}
		}
		if (FinaleActivated && ConversationTracker == 13) {
			if (MouseController::GetInstance()->IsButtonDown(GLFW_MOUSE_BUTTON_LEFT) && !ConversationBufferBool) {
				ConversationTracker = 16;
				ConversationBufferBool = true;
				havingconversation = false;
				ObjectiveText = ConversationText[ConversationTracker];
			}
		}
		if (MouseController::GetInstance()->IsButtonDown(GLFW_MOUSE_BUTTON_LEFT) && !ConversationBufferBool) {
			ConversationBufferBool = true;
			//std::cout << ConversationTracker << std::endl;
			if (ConversationTracker > 7 && FinaleActivated) {
				camera.Shake(0.5f, 1.f);
			}
			switch (ConversationTracker) {
			case 8:
				m_obj[8].AddImpulse(RandImpulse(500.f));
				break;
			case 9:
				m_obj[9].AddImpulse(RandImpulse(500.f));
				break;
			case 10:
				m_obj[10].AddImpulse(RandImpulse(500.f));
				break;
			case 11:
				m_obj[11].AddImpulse(RandImpulse(500.f));
				break;
			case 12:
				m_obj[12].AddImpulse(RandImpulse(500.f));
				break;
			default:
				break;
			}
			ConversationTracker += 1;
		}
		else if (!MouseController::GetInstance()->IsButtonDown(GLFW_MOUSE_BUTTON_LEFT) && ConversationBufferBool) {
			ConversationBufferBool = false;
		}
	}
	m_player.UpdatePhysics(dt);
	m_obj[8].UpdatePhysics(dt);
	m_obj[9].UpdatePhysics(dt);
	m_obj[10].UpdatePhysics(dt);
	m_obj[11].UpdatePhysics(dt);
	m_obj[12].UpdatePhysics(dt);
	m_player.vel *= pow(0.09f, dt);

	glm::vec3 offset(0.f, 10.f, 0.f);
	glm::vec3 newPos(m_player.pos.x, 0.f, m_player.pos.z);
	newPos += offset;

	glm::vec3 view = glm::normalize(camera.target - camera.position);

	if (!FinaleActivated) {
		camera.position = newPos;
		camera.target = camera.position + view;
	}
	else {
		camera.position = glm::vec3(0.f, 10.f, 10.f);
		camera.target = glm::vec3(m_obj[0].pos.x, 10.f, m_obj[0].pos.z);
	}
	camera.Update(dt);

	light[1].position = camera.position;
	light[1].spotDirection = glm::normalize(camera.position - camera.target);
	if (flashlighton) {
		light[1].power = 0.7f;
	}
	else {
		light[1].power = 0.f;
	}
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);

	CollisionData cd;

	for (wall& wall : m_wall) {
		if (OverlapCircle2OBB(m_player, 1.f, wall, wall.breadth, 1, cd)) {
			ResolveCollision(cd);
		}
	}

	for (obj& obj : m_obj) {
		//If obj is walk through-able
		if (obj.xLength == 0.f && obj.zLength == 0.f) {
			continue;
		}

		//Render Interaction tooltip
		if (obj.Interactable && OverlapCircle2OBB(m_player, 1, obj, obj.xLength + 9, obj.zLength + 9, cd) && !obj.InVisibleRange) {
			obj.InVisibleRange = true;
		}
		else {
			obj.InVisibleRange = false;
		}

		//Check if have interaction
		if (obj.InVisibleRange) {
			if (obj.Type == GEO_LIGHTSWITCH && KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_F))
			{
				interactingmenu = true;
				obj.Interacted = true;
			}
			else if (obj.Type != GEO_LIGHTSWITCH && ConversationPaper && KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_F)){
				interactingmenu = true;
				obj.Interacted = true;
			}
		}

		//Obj-Player collision
		if (OverlapCircle2OBB(m_player, 1.f, obj, obj.xLength, obj.zLength, cd)) {
			ResolveCollision(cd);
		}
	}

	for (obj& obj : m_obj) {
		if (obj.Interacted) {
			if (obj.Type == GEO_LIGHTSWITCH) {
				//std::cout << "HELLO" << std::endl;
				if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_Z))
				{
					interactingmenu = false;
					obj.Interacted = false;
					if (!ConversationPaper) {
						ConversationTracker += 1;
						havingconversation = true;
					}
				}
			}

			if (obj.Type == GEO_WINDOW) {
				if (WindowInteractionCount == 3) {
					ConversationTracker = 15;
				}
				else {
					ConversationTracker = 14;
				}
				havingconversation = true;
				if (MouseController::GetInstance()->IsButtonDown(GLFW_MOUSE_BUTTON_LEFT))
				{
					interactingmenu = false;
					obj.Interacted = false;
					obj.Interactable = false;
					ConversationTracker = 5;
					havingconversation = false;
					WindowInteractionCount += 1;
					if (WindowInteractionCount == 4) {
						havingconversation = true;
						ConversationTracker += 1;
					}
				}
			}

			if (obj.Type == GEO_CUPBOARD) {
				FinaleActivated = true;
				obj.Interacted = false;
				obj.Interactable = false;
				havingconversation = true;
				ConversationTracker += 1;
				camera.Shake(0.5f, 1.f);
			}
		}
	}

	if (interactingmenu) {
		if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_R))
		{
			interactingmenu = false;
		}
	}

	float temp = 1.f / dt;
	fps = glm::round(temp * 100.f) / 100.f;
}

void SceneAssignment::Render()
{
	// Clear color buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Load view matrix stack and set it with camera position, target position and up direction
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);

	// Load identity matrix into the model stack
	modelStack.LoadIdentity();

	if (light[0].type == Light::LIGHT_DIRECTIONAL)
	{
		glm::vec3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
		glm::vec3 lightDirection_cameraspace = viewStack.Top() * glm::vec4(lightDir, 0);
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, glm::value_ptr(lightDirection_cameraspace));
	}
	else if (light[0].type == Light::LIGHT_SPOT)
	{
		glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[0].position, 1);
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
		glm::vec3 spotDirection_cameraspace = viewStack.Top() * glm::vec4(light[0].spotDirection, 0);
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, glm::value_ptr(spotDirection_cameraspace));
	}
	else {
		// Calculate the light position in camera space
		glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[0].position, 1);
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
	}

	if (light[1].type == Light::LIGHT_SPOT)
	{
		glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[1].position, 1);
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
		glm::vec3 spotDirection_cameraspace = viewStack.Top() * glm::vec4(light[1].spotDirection, 0);
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, glm::value_ptr(spotDirection_cameraspace));
	}

	if (light[2].type == Light::LIGHT_POINT)
	{
		glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[2].position, 1);
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
	}


	//modelStack.PushMatrix();
	//// Render objects using new RenderMesh()
	//RenderMesh(meshList[GEO_AXES], false);
	//modelStack.PopMatrix();
	//modelStack.PushMatrix();
	//modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	//modelStack.Scale(0.1f, 0.1f, 0.1f);
	//RenderMesh(meshList[GEO_SPHERE], false);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(light[1].position.x, light[1].position.y, light[1].position.z);
	//modelStack.Scale(0.1f, 0.1f, 0.1f);
	//RenderMesh(meshList[GEO_SPHERE], false);
	//modelStack.PopMatrix();

	modelStack.PushMatrix();
	meshList[GEO_LEFT]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_LEFT]->material.kDiffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	meshList[GEO_LEFT]->material.kSpecular = glm::vec3(0.9f, 0.9f, 0.9f);
	meshList[GEO_LEFT]->material.kShininess = 1.0f;
	modelStack.Translate(0.f, 25.f, -100.f);
	modelStack.Rotate(0.f, 1.f, 0.f, 0.f);
	modelStack.Scale(2.f, 0.5f, 1.f);
	RenderMesh(meshList[GEO_LEFT], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	meshList[GEO_RIGHT]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_RIGHT]->material.kDiffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	meshList[GEO_RIGHT]->material.kSpecular = glm::vec3(0.9f, 0.9f, 0.9f);
	meshList[GEO_RIGHT]->material.kShininess = 1.0f;
	modelStack.Translate(0.f, 25.f, 100.f);
	modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
	modelStack.Scale(2.f, 0.5f, 1.f);
	RenderMesh(meshList[GEO_RIGHT], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	meshList[GEO_TOP]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_TOP]->material.kDiffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	meshList[GEO_TOP]->material.kSpecular = glm::vec3(0.9f, 0.9f, 0.9f);
	meshList[GEO_TOP]->material.kShininess = 1.0f;
	modelStack.Translate(0.f, 50.f, 0.f);
	modelStack.Rotate(90.f, 1.f, 0.f, 0.f);
	modelStack.Rotate(180.f, 0.f, 0.f, 1.f);
	modelStack.Scale(2.f, 2.f, 2.f);
	RenderMesh(meshList[GEO_TOP], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	meshList[GEO_BOTTOM]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_BOTTOM]->material.kDiffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	meshList[GEO_BOTTOM]->material.kSpecular = glm::vec3(0.9f, 0.9f, 0.9f);
	meshList[GEO_BOTTOM]->material.kShininess = 1.0f;
	modelStack.Translate(0.f, 0.f, 0.f);
	modelStack.Rotate(-90.f, 1.f, 0.f, 0.f);
	modelStack.Scale(2.f, 2.f, 2.f);
	RenderMesh(meshList[GEO_BOTTOM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	meshList[GEO_FRONT]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_FRONT]->material.kDiffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	meshList[GEO_FRONT]->material.kSpecular = glm::vec3(0.9f, 0.9f, 0.9f);
	meshList[GEO_FRONT]->material.kShininess = 1.0f;
	modelStack.Translate(100.f, 25.f, 0.f);
	modelStack.Rotate(-90.f, 0.f, 1.f, 0.f);
	modelStack.Scale(2.f, 0.5f, 1.f);
	RenderMesh(meshList[GEO_FRONT], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	meshList[GEO_BACK]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_BACK]->material.kDiffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	meshList[GEO_BACK]->material.kSpecular = glm::vec3(0.9f, 0.9f, 0.9f);
	meshList[GEO_BACK]->material.kShininess = 1.0f;
	modelStack.Translate(-100.f, 25.f, 0.f);
	modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
	modelStack.Scale(2.f, 0.5f, 1.f);
	RenderMesh(meshList[GEO_BACK], true);
	modelStack.PopMatrix();

	for (wall& wall : m_wall) {
		modelStack.PushMatrix();
		meshList[GEO_WALL]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
		meshList[GEO_WALL]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		meshList[GEO_WALL]->material.kSpecular = glm::vec3(0.9f, 0.9f, 0.9f);
		meshList[GEO_WALL]->material.kShininess = 5.0f;
		modelStack.Translate(wall.pos.x, wall.pos.y, wall.pos.z);
		modelStack.Rotate(wall.RotationDeg, wall.RotationAxis.x, wall.RotationAxis.y, wall.RotationAxis.z);
		modelStack.Scale(wall.Scale.x, wall.Scale.y, wall.Scale.z);
		RenderMesh(meshList[GEO_WALL], true);
		modelStack.PopMatrix();
	}

	for (obj& obj : m_obj) {
		modelStack.PushMatrix();
		meshList[obj.Type]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
		meshList[obj.Type]->material.kDiffuse = glm::vec3(0.7f, 0.7f, 0.7f);
		meshList[obj.Type]->material.kSpecular = glm::vec3(0.9f, 0.9f, 0.9f);
		meshList[obj.Type]->material.kShininess = 5.0f;
		modelStack.Translate(obj.pos.x, obj.pos.y, obj.pos.z);
		modelStack.Rotate(obj.RotationDeg, obj.RotationAxis.x, obj.RotationAxis.y, obj.RotationAxis.z);
		modelStack.Scale(obj.Scale.x, obj.Scale.y, obj.Scale.z);
		RenderMesh(meshList[obj.Type], true);
		modelStack.PopMatrix();

		if (obj.InVisibleRange) {

			glm::vec3 defaultForward(0.f, 0.f, -1.f); //In opengl framework, default forward facing pos is facing -Z

			glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(obj.RotationDeg), ConvertVec3(obj.RotationAxis));
											//Builds an identity matrix	   //Convert rotation to rad      //gets thr rotation axis
												  //This therefore means that this matrix stores the rotation around the specified axis

			// Forward direction		//Vec length = 1						//This tells it its a direction not pos
			glm::vec3 forwardDir = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(defaultForward, 0.f)));
			//Rotates the defaultforward based off the previous rotationmatrix to get the current forward facing in the world space

			// World up
			glm::vec3 up(0.f, 1.f, 0.f);

			// Right / Left directions			//takes two perpendicular values and returns a vector along the 3rd axis, which is perpendicular to both
			glm::vec3 rightDir = glm::normalize(glm::cross(forwardDir, up));
			//Gives rightdir first since OpenGL/GLM uses somthn like Right-Hand rule, index points to first passed-through value, thumb is result
			glm::vec3 leftDir = -rightDir;

			// Offsets
			float frontOffset = -0.8f;
			float leftOffset = 2.5f;
			float heightOffset = 1.0f;

			// Final world position
			glm::vec3 interactTextPos = ConvertVec3(obj.pos) + forwardDir * frontOffset + leftDir * leftOffset + glm::vec3(0.f, heightOffset, 0.f);
										//Change it into vec3	//Moves text forward		//Left				//Up


			if (obj.Type == GEO_LIGHTSWITCH) {
				modelStack.PushMatrix();
				modelStack.Translate(obj.pos.x - 3.f, obj.pos.y + 2.f, obj.pos.z + 0.5f);
				modelStack.Rotate(obj.RotationDeg + 90, obj.RotationAxis.x, obj.RotationAxis.y, obj.RotationAxis.z);
				modelStack.Scale(0.3f, 0.3f, 0.3f);
				RenderText(meshList[GEO_TEXT], "Press F to Interact", glm::vec3(0, 1, 0));
				modelStack.PopMatrix();
			}
			if (obj.Type == GEO_CUPBOARD && ConversationWindow) {
				modelStack.PushMatrix();
				modelStack.Translate(interactTextPos.x, interactTextPos.y + 7.5f, interactTextPos.z + 2);
				modelStack.Rotate(obj.RotationDeg, obj.RotationAxis.x, obj.RotationAxis.y, obj.RotationAxis.z);
				modelStack.Scale(0.3f, 0.3f, 0.3f);
				RenderText(meshList[GEO_TEXT], "Press F to Interact", glm::vec3(0, 1, 0));
				modelStack.PopMatrix();
			}
			else if (obj.Type != GEO_CUPBOARD && obj.Type != GEO_LIGHTSWITCH && ConversationPaper) {
				modelStack.PushMatrix();
				modelStack.Translate(interactTextPos.x, interactTextPos.y, interactTextPos.z);
				modelStack.Rotate(obj.RotationDeg, obj.RotationAxis.x, obj.RotationAxis.y, obj.RotationAxis.z);
				modelStack.Scale(0.3f, 0.3f, 0.3f);
				RenderText(meshList[GEO_TEXT], "Press F to Interact", glm::vec3(0, 1, 0));
				modelStack.PopMatrix();
			}
		}
	}

	//Paper
	modelStack.PushMatrix();
	meshList[GEO_PAPER]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_PAPER]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_PAPER]->material.kSpecular = glm::vec3(0.6f, 0.6f, 0.6f);
	meshList[GEO_PAPER]->material.kShininess = 5.0f;
	modelStack.Translate(m_obj[13].pos.x - 3, m_obj[13].pos.y, m_obj[13].pos.z + 0.2f);
	modelStack.Rotate(90.f, 0.f, 0.f, 1.f);
	modelStack.Rotate(90.f, 1.f, 0.f, 0.f);
	modelStack.Scale(0.02f, 0.02f, 0.02f);
	RenderMesh(meshList[GEO_PAPER], false);
	modelStack.PopMatrix();

	//PaperMsg
	if (m_obj[13].Interacted) {
		modelStack.PushMatrix();
		RenderMeshOnScreen(meshList[GEO_PAPERMSG], 400, 300, 400, 500);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		std::string temp("Press C for torch");
		RenderTextOnScreen(meshList[GEO_TEXT], temp, glm::vec3(0, 1.f, 0), 20, 0, 20);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		std::string temp2("Press Z to exit");
		RenderTextOnScreen(meshList[GEO_TEXT], temp2, glm::vec3(0, 0.5f, 0.5f), 20, 0, 0);
		modelStack.PopMatrix();
	}

	//Celing
	modelStack.PushMatrix();
	meshList[GEO_WALL]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_WALL]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_WALL]->material.kSpecular = glm::vec3(0.9f, 0.9f, 0.9f);
	meshList[GEO_WALL]->material.kShininess = 1.0f;
	modelStack.Translate(0.f + MainBuildingOffset.x, 20.f + MainBuildingOffset.y, 0.f + MainBuildingOffset.z);
	modelStack.Rotate(90.f, 1.f, 0.f, 0.f);
	modelStack.Scale(1.f, 1.f, 1.f);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	if (havingconversation) {
		modelStack.PushMatrix();
		RenderMeshOnScreen(meshList[GEO_TEXTBOX], 400, 50, 800, 190);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		if (!FinaleActivated) {
			RenderTextOnScreen(meshList[GEO_TEXT], ConversationText[ConversationTracker], glm::vec3(1, 1, 1), 18, 100, 50);
		}
		else {
			RenderTextOnScreen(meshList[GEO_TEXT], ConversationText[ConversationTracker], glm::vec3(1, 0, 0), 18, 100, 50);
		}
		modelStack.PopMatrix();
	}

	//modelStack.PushMatrix();
	//RenderText(meshList[GEO_TEXT], "Hello World", glm::vec3(0, 1, 0));
	//modelStack.PopMatrix();

	//FPS
	modelStack.PushMatrix();
	std::string temp("FPS:" + std::to_string(fps));
	RenderTextOnScreen(meshList[GEO_TEXT], temp.substr(0,9), glm::vec3(0, 1, 0), 20, 0, 580);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderTextOnScreen(meshList[GEO_TEXT], ObjectiveText, glm::vec3(0, 1, 0), 20, 0, 560);
	modelStack.PopMatrix();
}

void SceneAssignment::RenderMesh(Mesh* mesh, bool enableLight)
{
	glm::mat4 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, glm::value_ptr(MVP));
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, glm::value_ptr(modelView));
	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = glm::inverseTranspose(modelView);
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, glm::value_ptr(modelView_inverse_transpose));

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}

void SceneAssignment::NewObjInit(obj& obj, GEOMETRY_TYPE Type, Vector3 Scale, Vector3 pos, Vector3 RotationAxis, float RotationDeg, float mass, float xLength, bool Interactable)
{
	obj.Type = Type;
	obj.Scale = Scale;
	obj.pos = pos;
	obj.RotationAxis = RotationAxis;
	obj.RotationDeg = RotationDeg;
	obj.mass = mass;
	obj.xLength = xLength;
	obj.zLength = 1.f;

	//If can interact
	obj.Interactable = Interactable;
	//If player is inside hitbox
	obj.InVisibleRange = false;
	//If player has pressed F while in hitbox
	obj.Interacted = false;
	//If the menu has appeared
	obj.InteractionFired = false;

	obj.angleDeg = RotationDeg;
}


void SceneAssignment::NewObjInit(obj& obj, GEOMETRY_TYPE Type, Vector3 Scale, Vector3 pos, Vector3 RotationAxis, float RotationDeg, float mass, float xLength, float zLength, bool Interactable)
{
	obj.Type = Type;
	obj.Scale = Scale;
	obj.pos = pos;
	obj.RotationAxis = RotationAxis;
	obj.RotationDeg = RotationDeg;
	obj.mass = mass;
	obj.xLength = xLength;
	obj.zLength = zLength;
	obj.Interactable = Interactable;
	obj.InVisibleRange = false;
	obj.Interacted = false;
	obj.InteractionFired = false;

	obj.angleDeg = RotationDeg;
}

glm::vec3 SceneAssignment::ConvertVec3(Vector3 Vector)
{
	return glm::vec3(Vector.x, Vector.y, Vector.z);
}

Vector3 SceneAssignment::RandImpulse(float strength)
{
	float x = ((rand() % 2000) / 1000.0f) - 1.0f;
	float y = ((rand() % 2000) / 1000.0f) - 1.0f;
	float z = ((rand() % 2000) / 1000.0f) - 1.0f;

	Vector3 dir(x, y, z);

	// Normalize to get a direction, then scale by strength
	dir = dir.Normalized() * strength;

	return dir;
}

void SceneAssignment::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey)
{
	glDisable(GL_DEPTH_TEST);
	glm::mat4 ortho = glm::ortho(0.f, 800.f, 0.f, 600.f, -1000.f, 1000.f); // dimension of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 1.f);
	modelStack.Scale(sizex, sizey, 1.f);
	RenderMesh(mesh, false); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneAssignment::RenderTextOnScreen(Mesh* mesh, std::string text, glm::vec3 color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glm::mat4 ortho = glm::ortho(0.f, 800.f, 0.f, 600.f, - 100.f, 100.f); // dimension of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for orthomode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		glm::mat4 characterSpacing = glm::translate(
			glm::mat4(1.f),
			glm::vec3(0.5f + i * 1.0f, 0.5f, 0)
		);
		glm::mat4 MVP = projectionStack.Top() *
			viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
			glm::value_ptr(MVP));
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void SceneAssignment::RenderText(Mesh* mesh, std::string text, glm::vec3 color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Disable back face culling
	glDisable(GL_CULL_FACE);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		glm::mat4 characterSpacing = glm::translate(
			glm::mat4(1.f),
			glm::vec3(i * 1.0f, 0, 0));
		glm::mat4 MVP = projectionStack.Top() * viewStack.Top() *
			modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
			glm::value_ptr(MVP));
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}


void SceneAssignment::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
		{
			delete meshList[i];
		}
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void SceneAssignment::HandleKeyPress()
{
	if (KeyboardController::GetInstance()->IsKeyPressed(0x31))
	{
		// Key press to enable culling
		glEnable(GL_CULL_FACE);
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(0x32))
	{
		// Key press to disable culling
		glDisable(GL_CULL_FACE);
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(0x33))
	{
		// Key press to enable fill mode for the polygon
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(0x34))
	{
		// Key press to enable wireframe mode for the polygon
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
	{
		// Change to black background
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	}

	//if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_0))
	//{
	//	// Toggle light on or off
	///*	enableLight = !enableLight;*/

	//	if (light[0].power <= 0.1f)
	//		light[0].power = 1.f;
	//	else
	//		light[0].power = 0.1f;
	//	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	//}

	if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_TAB))
	{
		if (light[0].type == Light::LIGHT_POINT) {
			light[0].type = Light::LIGHT_DIRECTIONAL;
		}
		else if (light[0].type == Light::LIGHT_DIRECTIONAL) {
			light[0].type = Light::LIGHT_SPOT;
		}
		else {
			light[0].type = Light::LIGHT_POINT;
		}

		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_P))
	{
		std::cout << m_player.pos << std::endl;
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_C) && flashlightobtained)
	{
		flashlighton = !flashlighton;
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_B))
	{
		FinaleActivated = true;
	}
}

void SceneAssignment::HandleMouseInput()
{
	static bool isLeftUp = false;
	static bool isRightUp = false;
	// Process Left button
	if (!isLeftUp && MouseController::GetInstance() -> IsButtonDown(GLFW_MOUSE_BUTTON_LEFT))
	{
		isLeftUp = true;
		//std::cout << "LBUTTON DOWN" << std::endl;

		// Step 3
		// transform into UI space
		double x = MouseController::GetInstance() -> GetMousePositionX();
		double y = 600 - MouseController::GetInstance() -> GetMousePositionY();
		// Check if mouse click position is within the GUI box
		// Change the boundaries as necessary
		if (x > 0 && x < 150 && y > -200 && y < -50) {
			std::cout << "GUI IS CLICKED" << std::endl;
		}
		// End of step 
	}
	else if (isLeftUp && MouseController::GetInstance() -> IsButtonUp(GLFW_MOUSE_BUTTON_LEFT))
	{
		isLeftUp = false;
		//std::cout << "LBUTTON UP" << std::endl;
	}
	// Continue to do for right button
	if (!isRightUp && MouseController::GetInstance()->IsButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
	{
		isRightUp = true;
		//std::cout << "RBUTTON DOWN" << std::endl;

		// Step 3
		// transform into UI space
		double x = MouseController::GetInstance()->GetMousePositionX();
		double y = 600 - MouseController::GetInstance()->GetMousePositionY();
		// Check if mouse click position is within the GUI box
		// Change the boundaries as necessary
		std::cout << "X: " << x << " Y: " << y << std::endl;
		if (x >= 0 && x <= 150 && y >= -200 && y <= -50) {
			std::cout << "GUI IS CLICKED" << std::endl;
		}
		// End of step 
	}
	else if (isRightUp && MouseController::GetInstance()->IsButtonUp(GLFW_MOUSE_BUTTON_RIGHT))
	{
		isRightUp = false;
		//std::cout << "RBUTTON UP" << std::endl;
	}

}
