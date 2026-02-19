#ifndef SCENE_ASSIGNEMNT_H
#define SCENE_ASSIGNEMNT_H

#include "Scene.h"
#include "Mesh.h"
#include "AltAzCamera.h"
#include "MatrixStack.h"
#include "Light.h"
#include <iostream>
#include "MouseController.h"


#include "PhysicsObject.h"
#include "CollisionDetection.h"


#include "FPCamera.h"

class SceneAssignment : public Scene
{
public:
	enum GEOMETRY_TYPE
	{
		//Skybox
		GEO_AXES,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,

		//OBJs
		GEO_DOOR,
		GEO_WINDOW,
		GEO_TABLE,
		GEO_BED,
		GEO_CUPBOARD,
		GEO_BOARD,
		GEO_LIGHT,
		GEO_LIGHTSWITCH,
		GEO_PAPER,

		//TGAs
		GEO_WALL,
		GEO_PAPERMSG,

		//Others
		GEO_SPHERE,
		GEO_GUI,
		GEO_TEXTBOX,
		GEO_TEXT,
		NUM_GEOMETRY,
	};

	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,

		U_LIGHT1_TYPE,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,

		U_LIGHT2_TYPE,
		U_LIGHT2_POSITION,
		U_LIGHT2_COLOR,
		U_LIGHT2_POWER,
		U_LIGHT2_KC,
		U_LIGHT2_KL,
		U_LIGHT2_KQ,
		U_LIGHT2_SPOTDIRECTION,
		U_LIGHT2_COSCUTOFF,
		U_LIGHT2_COSINNER,
		U_LIGHT2_EXPONENT,

		U_NUMLIGHTS,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_LIGHTENABLED,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};

	struct wall : public PhysicsObject
	{
		Vector3 Scale;
		Vector3 RotationAxis;
		float RotationDeg;
		float height;
		float breadth;
	};

	struct obj : public PhysicsObject
	{
		GEOMETRY_TYPE Type;
		Vector3 Scale;
		Vector3 RotationAxis;
		float RotationDeg;
		float xLength;
		float zLength;
		bool Interactable;
		bool InVisibleRange;
		bool Interacted;
		bool InteractionFired;
	};

	SceneAssignment();
	~SceneAssignment();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	void HandleKeyPress();
	void HandleMouseInput();
	void RenderMesh(Mesh* mesh, bool enableLight);

	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	FPCamera camera;
	int projType = 1; // fix to 0 for orthographic, 1 for projection

	MatrixStack modelStack, viewStack, projectionStack;

	static const int NUM_LIGHTS = 3;
	Light light[NUM_LIGHTS];
	bool enableLight;

	float fps = 0;

	PhysicsObject m_player;
	wall m_wall[4];
	obj m_obj[15];

	void NewObjInit(obj& obj, GEOMETRY_TYPE Type, Vector3 Scale, Vector3 pos, Vector3 RotationAxis, float RotationDeg, float mass, float xLength, bool Interactable);
	void NewObjInit(obj& obj, GEOMETRY_TYPE Type, Vector3 Scale, Vector3 pos, Vector3 RotationAxis, float RotationDeg, float mass, float xLength, float zLength, bool Interactable);

	Vector3 MainBuildingOffset;
	bool havingconversation;
	bool interactingmenu;

	std::string ConversationText[17];
	std::string ObjectiveText;
	int ConversationTracker;
	int WindowInteractionCount;
	bool ConversationBufferBool;
	bool ConversationIntro;
	bool ConversationPaper;
	bool ConversationWindow;

	bool flashlighton;
	bool flashlightobtained;

	bool FinaleActivated;

	glm::vec3 ConvertVec3(Vector3 Vector);
	Vector3 RandImpulse(float strength);

	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey);
	void RenderTextOnScreen(Mesh* mesh, std::string text, glm::vec3 color, float size, float x, float y);
	void RenderText(Mesh* mesh, std::string text, glm::vec3 color);
};

#endif