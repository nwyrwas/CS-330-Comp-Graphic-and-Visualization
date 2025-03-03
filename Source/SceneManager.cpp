///////////////////////////////////////////////////////////////////////////////
// scenemanager.cpp
// ============
// manage the preparing and rendering of 3D scenes - textures, materials, lighting
//
//  AUTHOR: Brian Battersby - SNHU Instructor / Computer Science
//	Created for CS-330-Computational Graphics and Visualization, Nov. 1st, 2023
///////////////////////////////////////////////////////////////////////////////

#include "SceneManager.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#include <glm/gtx/transform.hpp>

// declaration of global variables
namespace
{
	const char* g_ModelName = "model";
	const char* g_ColorValueName = "objectColor";
	const char* g_TextureValueName = "objectTexture";
	const char* g_UseTextureName = "bUseTexture";
	const char* g_UseLightingName = "bUseLighting";
}

/***********************************************************
 *  SceneManager()
 *
 *  The constructor for the class
 ***********************************************************/
SceneManager::SceneManager(ShaderManager *pShaderManager)
{
	m_pShaderManager = pShaderManager;
	m_basicMeshes = new ShapeMeshes();

	for (int i = 0; i < 16; i++)
	{
		m_textureIDs[i].tag = "/0";
		m_textureIDs[i].ID = -1;
	}
	m_loadedTextures = 0;
}

/***********************************************************
 *  ~SceneManager()
 *
 *  The destructor for the class
 ***********************************************************/
SceneManager::~SceneManager()
{
	// clear the allocated memory
	m_pShaderManager = NULL;
	delete m_basicMeshes;
	m_basicMeshes = NULL;
	// destroy the created OpenGL textures
	DestroyGLTextures();
}

/***********************************************************
 *  CreateGLTexture()
 *
 *  This method is used for loading textures from image files,
 *  configuring the texture mapping parameters in OpenGL,
 *  generating the mipmaps, and loading the read texture into
 *  the next available texture slot in memory.
 ***********************************************************/
bool SceneManager::CreateGLTexture(const char* filename, std::string tag)
{
	int width = 0;
	int height = 0;
	int colorChannels = 0;
	GLuint textureID = 0;

	// indicate to always flip images vertically when loaded
	stbi_set_flip_vertically_on_load(true);

	// try to parse the image data from the specified image file
	unsigned char* image = stbi_load(
		filename,
		&width,
		&height,
		&colorChannels,
		0);

	// if the image was successfully read from the image file
	if (image)
	{
		std::cout << "Successfully loaded image:" << filename << ", width:" << width << ", height:" << height << ", channels:" << colorChannels << std::endl;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if the loaded image is in RGB format
		if (colorChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		// if the loaded image is in RGBA format - it supports transparency
		else if (colorChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		else
		{
			std::cout << "Not implemented to handle image with " << colorChannels << " channels" << std::endl;
			return false;
		}

		// generate the texture mipmaps for mapping textures to lower resolutions
		glGenerateMipmap(GL_TEXTURE_2D);

		// free the image data from local memory
		stbi_image_free(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

		// register the loaded texture and associate it with the special tag string
		m_textureIDs[m_loadedTextures].ID = textureID;
		m_textureIDs[m_loadedTextures].tag = tag;
		m_loadedTextures++;

		return true;
	}

	std::cout << "Could not load image:" << filename << std::endl;

	// Error loading the image
	return false;
}

/***********************************************************
 *  BindGLTextures()
 *
 *  This method is used for binding the loaded textures to
 *  OpenGL texture memory slots.  There are up to 16 slots.
 ***********************************************************/
void SceneManager::BindGLTextures()
{
	for (int i = 0; i < m_loadedTextures; i++)
	{
		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_textureIDs[i].ID);
	}
}

/***********************************************************
 *  DestroyGLTextures()
 *
 *  This method is used for freeing the memory in all the
 *  used texture memory slots.
 ***********************************************************/
void SceneManager::DestroyGLTextures()
{
	for (int i = 0; i < m_loadedTextures; i++)
	{
		glGenTextures(1, &m_textureIDs[i].ID);
	}
}

/***********************************************************
 *  FindTextureID()
 *
 *  This method is used for getting an ID for the previously
 *  loaded texture bitmap associated with the passed in tag.
 ***********************************************************/
int SceneManager::FindTextureID(std::string tag)
{
	int textureID = -1;
	int index = 0;
	bool bFound = false;

	while ((index < m_loadedTextures) && (bFound == false))
	{
		if (m_textureIDs[index].tag.compare(tag) == 0)
		{
			textureID = m_textureIDs[index].ID;
			bFound = true;
		}
		else
			index++;
	}

	return(textureID);
}

/***********************************************************
 *  FindTextureSlot()
 *
 *  This method is used for getting a slot index for the previously
 *  loaded texture bitmap associated with the passed in tag.
 ***********************************************************/
int SceneManager::FindTextureSlot(std::string tag)
{
	int textureSlot = -1;
	int index = 0;
	bool bFound = false;

	while ((index < m_loadedTextures) && (bFound == false))
	{
		if (m_textureIDs[index].tag.compare(tag) == 0)
		{
			textureSlot = index;
			bFound = true;
		}
		else
			index++;
	}

	return(textureSlot);
}

/***********************************************************
 *  SetTransformations()
 *
 *  This method is used for setting the transform buffer
 *  using the passed in transformation values.
 ***********************************************************/
void SceneManager::SetTransformations(
	glm::vec3 scaleXYZ,
	float XrotationDegrees,
	float YrotationDegrees,
	float ZrotationDegrees,
	glm::vec3 positionXYZ)
{
	// variables for this method
	glm::mat4 modelView;
	glm::mat4 scale;
	glm::mat4 rotationX;
	glm::mat4 rotationY;
	glm::mat4 rotationZ;
	glm::mat4 translation;

	// set the scale value in the transform buffer
	scale = glm::scale(scaleXYZ);
	// set the rotation values in the transform buffer
	rotationX = glm::rotate(glm::radians(XrotationDegrees), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationY = glm::rotate(glm::radians(YrotationDegrees), glm::vec3(0.0f, 1.0f, 0.0f));
	rotationZ = glm::rotate(glm::radians(ZrotationDegrees), glm::vec3(0.0f, 0.0f, 1.0f));
	// set the translation value in the transform buffer
	translation = glm::translate(positionXYZ);

	modelView = translation * rotationZ * rotationY * rotationX * scale;

	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setMat4Value(g_ModelName, modelView);
	}
}

/***********************************************************
 *  SetShaderColor()
 *
 *  This method is used for setting the passed in color
 *  into the shader for the next draw command
 ***********************************************************/
void SceneManager::SetShaderColor(
	float redColorValue,
	float greenColorValue,
	float blueColorValue,
	float alphaValue)
{
	// variables for this method
	glm::vec4 currentColor;

	currentColor.r = redColorValue;
	currentColor.g = greenColorValue;
	currentColor.b = blueColorValue;
	currentColor.a = alphaValue;

	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setIntValue(g_UseTextureName, false);
		m_pShaderManager->setVec4Value(g_ColorValueName, currentColor);
	}
}

/***********************************************************
 *  SetShaderTexture()
 *
 *  This method is used for setting the texture data
 *  associated with the passed in ID into the shader.
 ***********************************************************/
void SceneManager::SetShaderTexture(
	std::string textureTag)
{
	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setIntValue(g_UseTextureName, true);

		int textureID = -1;
		textureID = FindTextureSlot(textureTag);
		m_pShaderManager->setSampler2DValue(g_TextureValueName, textureID);
	}
}

/***********************************************************
 *  SetTextureUVScale()
 *
 *  This method is used for setting the texture UV scale
 *  values into the shader.
 ***********************************************************/
void SceneManager::SetTextureUVScale(float u, float v)
{
	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setVec2Value("UVscale", glm::vec2(u, v));
	}
}



/**************************************************************/
/*** STUDENTS CAN MODIFY the code in the methods BELOW for  ***/
/*** preparing and rendering their own 3D replicated scenes.***/
/*** Please refer to the code in the OpenGL sample project  ***/
/*** for assistance.                                        ***/
/**************************************************************/
bool SceneManager::FindMaterial(std::string tag, OBJECT_MATERIAL& material)
{
	if (m_objectMaterials.size() == 0)
	{
		return(false);
	}

	int index = 0;
	bool bFound = false;
	while ((index < m_objectMaterials.size()) && (bFound == false))
	{
		if (m_objectMaterials[index].tag.compare(tag) == 0)
		{
			bFound = true;
			material.diffuseColor = m_objectMaterials[index].diffuseColor;
			material.specularColor = m_objectMaterials[index].specularColor;
			material.shininess = m_objectMaterials[index].shininess;
		}
		else
		{
			index++;
		}
	}

	return(true);
}


/***********************************************************
 *  SetShaderMaterial()
 *
 *  This method is used for passing the material values
 *  into the shader.
 ***********************************************************/
void SceneManager::SetShaderMaterial(
	std::string materialTag)
{
	if (m_objectMaterials.size() > 0)
	{
		OBJECT_MATERIAL material;
		bool bReturn = false;

		// find the defined material that matches the tag
		bReturn = FindMaterial(materialTag, material);
		if (bReturn == true)
		{
			// pass the material properties into the shader
			m_pShaderManager->setVec3Value("material.diffuseColor", material.diffuseColor);
			m_pShaderManager->setVec3Value("material.specularColor", material.specularColor);
			m_pShaderManager->setFloatValue("material.shininess", material.shininess);
		}
	}
}




 /***********************************************************
  *  LoadSceneTextures()
  *
  *  This method is used for preparing the 3D scene by loading
  *  the shapes, textures in memory to support the 3D scene
  *  rendering
  ***********************************************************/

void SceneManager::LoadSceneTextures() {

	bool bReturn = false;


	//here I will be adding textures to the phone holder as it uses both box meshes and a cylinder 

	//silver base for the base
	bReturn = CreateGLTexture(
		"textures/silverBase.jpg", 
		"silverBase");

	//metallic silver for the cylinder
	bReturn = CreateGLTexture(
		"textures/metallicSilver.jpg", 
		"metallicSilver");

	//rubber gray texture for the holder part
	bReturn = CreateGLTexture(
		"textures/rubber.jpg", 
		"rubber");

	bReturn = CreateGLTexture(
		"textures/grayHolders.jpg", 
		"grayHolder");

	bReturn = CreateGLTexture(
		"textures/rusticwood.jpg", 
		"wood");

	bReturn = CreateGLTexture(
		"textures/knife_handle.jpg", 
		"knife");

	bReturn = CreateGLTexture(
		"textures/stainedglass.jpg", 
		"stained");

	bReturn = CreateGLTexture(
		"textures/backdrop.jpg",
		"backdrop");


	bReturn = CreateGLTexture(
		"textures/keyboardBase.jpg",
		"keyboard");

	bReturn = CreateGLTexture(
		"textures/mouse.jpg",
		"mouse");

	bReturn = CreateGLTexture(
		"textures/waterBottle.jpg",
		"waterBottle");

	bReturn = CreateGLTexture(
		"textures/book.jpg",
		"book");

	bReturn = CreateGLTexture(
		"textures/drywall.jpg",
		"body");

	BindGLTextures();
}

/***********************************************************
 *  DefineObjectMaterials()
 *
 *  This method is used for configuring the various material
 *  settings for all of the objects within the 3D scene.
 ***********************************************************/
void SceneManager::DefineObjectMaterials()
{
	OBJECT_MATERIAL goldMaterial;
	goldMaterial.diffuseColor = glm::vec3(0.8f, 0.8f, 0.0f); // Increase diffuse color
	goldMaterial.specularColor = glm::vec3(1.0f, 1.0f, 0.8f); // Increase specular color
	goldMaterial.shininess = 60.0;
	goldMaterial.tag = "metal";

	m_objectMaterials.push_back(goldMaterial);

	OBJECT_MATERIAL woodMaterial;
	woodMaterial.diffuseColor = glm::vec3(0.4f, 0.2f, 0.1f); // Increase diffuse color
	woodMaterial.specularColor = glm::vec3(0.2f, 0.2f, 0.2f); // Increase specular color
	woodMaterial.shininess = 0.1;
	woodMaterial.tag = "wooden";

	m_objectMaterials.push_back(woodMaterial);

	OBJECT_MATERIAL glassMaterial;
	glassMaterial.diffuseColor = glm::vec3(0.2f, 0.2f, 0.2f);
	glassMaterial.specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glassMaterial.shininess = 95.0;
	glassMaterial.tag = "glass";

	m_objectMaterials.push_back(glassMaterial);

	OBJECT_MATERIAL plateMaterial;
	plateMaterial.diffuseColor = glm::vec3(0.4f, 0.4f, 0.4f);
	plateMaterial.specularColor = glm::vec3(0.2f, 0.2f, 0.2f);
	plateMaterial.shininess = 30.0;
	plateMaterial.tag = "plate";

	m_objectMaterials.push_back(plateMaterial);

	OBJECT_MATERIAL rubberMaterial;
	rubberMaterial.diffuseColor = glm::vec3(0.7f, 0.6f, 0.5f);
	rubberMaterial.specularColor = glm::vec3(0.02f, 0.02f, 0.02f);
	rubberMaterial.shininess = 0.001;
	rubberMaterial.tag = "rubber";

	// Define book material
	OBJECT_MATERIAL bookMaterial;
	bookMaterial.diffuseColor = glm::vec3(0.6f, 0.3f, 0.1f); // Brownish color for the book cover
	bookMaterial.specularColor = glm::vec3(0.2f, 0.2f, 0.2f); // Low specular color
	bookMaterial.shininess = 10.0; // Low shininess
	bookMaterial.tag = "book";

	m_objectMaterials.push_back(bookMaterial);
}


void SceneManager::SetupSceneLights()
{
	// this line of code is NEEDED for telling the shaders to render 
	// the 3D scene with custom lighting - to use the default rendered 
	// lighting then comment out the following line
	m_pShaderManager->setBoolValue(g_UseLightingName, true);

	// directional light to emulate sunlight coming into scene
	m_pShaderManager->setVec3Value("directionalLight.direction", -0.05f, -0.3f, -0.1f);
	m_pShaderManager->setVec3Value("directionalLight.ambient", 0.05f, 0.05f, 0.05f);
	m_pShaderManager->setVec3Value("directionalLight.diffuse", 0.6f, 0.6f, 0.6f);
	m_pShaderManager->setVec3Value("directionalLight.specular", 0.0f, 0.0f, 0.0f);
	m_pShaderManager->setBoolValue("directionalLight.bActive", true);

	// point light 1
	m_pShaderManager->setVec3Value("pointLights[0].position", -4.0f, 8.0f, 0.0f);
	m_pShaderManager->setVec3Value("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
	m_pShaderManager->setVec3Value("pointLights[0].diffuse", 0.3f, 0.3f, 0.3f);
	m_pShaderManager->setVec3Value("pointLights[0].specular", 0.1f, 0.1f, 0.1f);
	m_pShaderManager->setBoolValue("pointLights[0].bActive", true);
	// point light 2
	m_pShaderManager->setVec3Value("pointLights[1].position", 4.0f, 8.0f, 0.0f);
	m_pShaderManager->setVec3Value("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
	m_pShaderManager->setVec3Value("pointLights[1].diffuse", 0.3f, 0.3f, 0.3f);
	m_pShaderManager->setVec3Value("pointLights[1].specular", 0.1f, 0.1f, 0.1f);
	m_pShaderManager->setBoolValue("pointLights[1].bActive", true);
	// point light 3
	m_pShaderManager->setVec3Value("pointLights[2].position", 3.8f, 5.5f, 4.0f);
	m_pShaderManager->setVec3Value("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
	m_pShaderManager->setVec3Value("pointLights[2].diffuse", 0.2f, 0.2f, 0.2f);
	m_pShaderManager->setVec3Value("pointLights[2].specular", 0.8f, 0.8f, 0.8f);
	m_pShaderManager->setBoolValue("pointLights[2].bActive", true);
	


	// point light 5

	//used to illuminate the backdrop
	m_pShaderManager->setVec3Value("pointLights[4].position", -3.2f, 6.0f, -4.0f);
	m_pShaderManager->setVec3Value("pointLights[4].ambient", 0.05f, 0.05f, 0.05f);
	m_pShaderManager->setVec3Value("pointLights[4].diffuse", 0.9f, 0.9f, 0.9f);
	m_pShaderManager->setVec3Value("pointLights[4].specular", 0.1f, 0.1f, 0.1f);
	m_pShaderManager->setBoolValue("pointLights[4].bActive", true);


	// Spotlight to cover all objects
	m_pShaderManager->setVec3Value("spotLight.position", 0.0f, 10.0f, 0.0f); // Position above the center of the scene
	m_pShaderManager->setVec3Value("spotLight.direction", 0.0f, -1.0f, 0.0f); // Pointing downwards
	m_pShaderManager->setVec3Value("spotLight.ambient", 0.8f, 0.8f, 0.8f);
	m_pShaderManager->setVec3Value("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	m_pShaderManager->setVec3Value("spotLight.specular", 0.7f, 0.7f, 0.7f);
	m_pShaderManager->setFloatValue("spotLight.constant", 1.0f);
	m_pShaderManager->setFloatValue("spotLight.linear", 0.09f);
	m_pShaderManager->setFloatValue("spotLight.quadratic", 0.032f);
	m_pShaderManager->setFloatValue("spotLight.cutOff", glm::cos(glm::radians(45.0f))); // Wide cutoff angle
	m_pShaderManager->setFloatValue("spotLight.outerCutOff", glm::cos(glm::radians(50.0f))); // Wide outer cutoff angle
	m_pShaderManager->setBoolValue("spotLight.bActive", true);
}




/***********************************************************
 *  PrepareScene()
 *
 *  This method is used for preparing the 3D scene by loading
 *  the shapes, textures in memory to support the 3D scene 
 *  rendering
 ***********************************************************/
void SceneManager::PrepareScene()
{	
	//loads the textures from textures folder for scene
	LoadSceneTextures();


	// define the materials for the objects in the scene
	DefineObjectMaterials();


	// set the lighting for the scene
	SetupSceneLights();


	// only one instance of a particular mesh needs to be
	// loaded in memory no matter how many times it is drawn
	// in the rendered 3D scene

	m_basicMeshes->LoadPlaneMesh();
	m_basicMeshes->LoadCylinderMesh();
	m_basicMeshes->LoadBoxMesh();
	m_basicMeshes->LoadSphereMesh();
	m_basicMeshes->LoadPyramid4Mesh();
	m_basicMeshes->LoadConeMesh();
	m_basicMeshes->LoadTaperedCylinderMesh();
	

}

/***********************************************************
 *  RenderScene()
 *
 *  This method is used for rendering the 3D scene by 
 *  transforming and drawing the basic 3D shapes
 ***********************************************************/
void SceneManager::RenderScene()
{

	//This will render the objects for each section of the scene
	RenderWaterBottle();
	RenderBackDrop();
	RenderPhoneHolder();
	RenderDesk();
	RenderBook();
	RenderMonitors();
	RenderMouse();
	RenderKeyBoard();
}

void SceneManager::RenderBackDrop() {
	glm::vec3 scaleXYZ;
	float XrotationDegrees = 0.0f;
	float YrotationDegrees = 0.0f;
	float ZrotationDegrees = 0.0f;
	glm::vec3 positionXYZ;

	// Set needed transformations before drawing the basic mesh
	scaleXYZ = glm::vec3(50.0f, 50.0f, 50.0f);
	XrotationDegrees = 90.0f;
	positionXYZ = glm::vec3(0.0f, 0.0f, -10.0f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	SetShaderTexture("backdrop");
	SetTextureUVScale(1.0, 1.0);

	m_basicMeshes->DrawPlaneMesh();
}

void SceneManager::RenderDesk() {
	/*********************************************************
	* This section will create a 3D plane for the 3D scene
	* This will be a desk consisting of a desk using box meshes to create the 3D shape
	**********************************************************/

	//This part will be the top of the desk that the objects will be on top off
	glm::vec3 scaleXYZ;
	float XrotationDegrees = 0.0f;
	float YrotationDegrees = 0.0f;
	float ZrotationDegrees = 0.0f;
	glm::vec3 positionXYZ;

	// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(40.0f, 1.0f, 20.0f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3(0.0f, -0.5f, 0.0f);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	SetShaderColor(1, 1, 1, 1);

	SetShaderTexture("knife");
	SetTextureUVScale(1.0, 1.0);

	//setting object shader
	SetShaderMaterial("wooden");


	// draw the mesh with transformation values
	m_basicMeshes->DrawBoxMesh();
	// set the color for the next draw command





	//this section will be two box meshes will be used to create the bottom part of the desk
	//based on how the desk is made


	//left side box mesh
	// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(8.0f, 12.0f, 10.0f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3(-15.0f, -6.5f, 0.0f);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	SetShaderColor(1, 1, 1, 1);

	SetShaderTexture("wood");
	SetTextureUVScale(1.0, 1.0);

	//setting object shader
	SetShaderMaterial("wooden");

	// draw the mesh with transformation values
	m_basicMeshes->DrawBoxMesh();
	// set the color for the next draw command



	//right side box mesh
	// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(8.0f, 12.0f, 10.0f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3(15.0f, -6.5f, 0.0f);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	SetShaderColor(1, 1, 1, 1);
	SetShaderTexture("wood");
	SetTextureUVScale(1.0, 1.0);


	// set the object material into the shader
	SetShaderMaterial("wooden");

	// draw the mesh with transformation values
	m_basicMeshes->DrawBoxMesh();
	// set the color for the next draw command
}

void SceneManager::RenderPhoneHolder() {
	/****************************************************************
	*   This part will start creating the phone holder for the 2D Image
	*
	*	This is where I will be using multiple shapes to create the object
	*
	*		*Shapes such as:
	*			*Box mesh for the base, and connector between the body and the holder section (Where the phone goes)
	*				*Two box meshes for the bottom of the holder section and for the body of the holder section
	*
	*			*Cylinder mesh for the body
	*****************************************************************/


	/****************************************************************
	*	Section that will be working on the base of the object
	*	I will be using the box mesh and flatten it on the y axis and elongate it a little bit on the x and z axis
	*	I will then rotate it slightly at a -45 degree angle on the y axis to reference the photo proposed in the 2-3 milestone
	****************************************************************/

	glm::vec3 scaleXYZ;
	float XrotationDegrees = 0.0f;
	float YrotationDegrees = 0.0f;
	float ZrotationDegrees = 0.0f;
	glm::vec3 positionXYZ;

	//Size of the box mesh to replicate the phone holder
	scaleXYZ = glm::vec3(2.0f, 0.2f, 2.0f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = -45.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3(1.0f, 0.0f, 1.0f);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//setting texture for the object
	SetShaderTexture("silverBase");
	SetTextureUVScale(1.0, 1.0);

	// draw the mesh with transformation values
	m_basicMeshes->DrawBoxMesh();


	/*******************************************************************
	*	This section will cover the body of the phone holder
	*	Where I will be using a cylinder mesh that is standing up and being elongated
	*	I will be stretching out the sides giving it an oval type shape that would match what it looks like on the reference photo
	********************************************************************/

	//Size of the cylinder for the body while also having an elongated height and oval shape
	scaleXYZ = glm::vec3(0.15f, 3.0f, 0.1f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = -45.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3(1.5f, 0.0f, 0.5f);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//setting texture for the object
	SetShaderTexture("metallicSilver");
	SetTextureUVScale(1.0, 1.0);

	// set object shader
	SetShaderMaterial("glass");

	// draw the mesh with transformation values
	m_basicMeshes->DrawCylinderMesh();




	/*******************************************************************
	*	This section will cover the connecting part between the holder section on the phone holder
	*	And the connecting part from the holder to the body
	*
	*
	*	I will be using a box as the connector
	*	Then a cylinder to start the top of the holder
	*	A box to act as the body of the holder
	*	Finally box meshes for the parts at the bottom that hold the phone in place
	*
	********************************************************************/

	//Box Mesh that connects to the cylinder body
	scaleXYZ = glm::vec3(0.2f, 0.2f, 0.2f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = -45.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3(1.4f, 2.8f, 0.6f);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//setting texture for the object
	SetShaderTexture("silverBase");
	SetTextureUVScale(1.0, 1.0);

	// setting object shader
	SetShaderMaterial("glass");

	// draw the mesh with transformation values
	m_basicMeshes->DrawBoxMesh();



	/************************************************************
	*	Section that will start the body of the holder section
	*************************************************************/

	//Cylinder Mesh that starts the top of the holder section
	scaleXYZ = glm::vec3(0.1f, 1.5f, 0.1f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 90.0f;
	YrotationDegrees = 45.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3(0.78f, 2.8f, 0.1f);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//setting texture for the object
	SetShaderTexture("body");
	SetTextureUVScale(1.0, 1.0);

	// setting object shader
	SetShaderMaterial("glass");

	// draw the mesh with transformation values
	m_basicMeshes->DrawCylinderMesh();




	//box mesh that acts as the body for the holder section
	scaleXYZ = glm::vec3(1.5f, 0.05f, 2.0f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 60.0f;
	YrotationDegrees = -45.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3(0.95f, 2.0f, 0.99065f);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//setting texture for the object
	SetShaderTexture("drywall");
	SetTextureUVScale(1.0, 1.0);

	// setting object shader
	SetShaderMaterial("glass");

	// draw the mesh with transformation values
	m_basicMeshes->DrawBoxMesh();



	/******************************************
	*
	*	Four box meshes that will act as the bottom of the holder section
	*
	*******************************************/


	//box mesh that acts as the bottom of the body for the holder (left side)
	scaleXYZ = glm::vec3(0.2f, 0.05f, 0.2f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = -20.0f;
	YrotationDegrees = -45.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3(0.12f, 1.23f, 1.0f);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//setting texture for the object
	SetShaderTexture("grayHolder");
	SetTextureUVScale(1.0, 1.0);

	// setting object shader
	SetShaderMaterial("plate");


	// draw the mesh with transformation values
	m_basicMeshes->DrawBoxMesh();



	//box mesh that acts as the bottom of the body for the holder (right side)
	scaleXYZ = glm::vec3(0.2f, 0.05f, 0.2f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = -20.0f;
	YrotationDegrees = -45.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3(0.95f, 1.23f, 1.8f);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//setting texture for the object
	SetShaderTexture("grayHolder");
	SetTextureUVScale(1.0, 1.0);

	// setting object shader
	SetShaderMaterial("plate");

	// draw the mesh with transformation values
	m_basicMeshes->DrawBoxMesh();

	//box mesh that acts as the bottom of the body for the holder (left side)
	scaleXYZ = glm::vec3(0.2f, 0.05f, 0.2f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = -125.0f;
	YrotationDegrees = -45.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3(0.12f, 1.35f, 1.0f);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//setting texture for the object
	SetShaderTexture("grayHolder");
	SetTextureUVScale(1.0, 1.0);

	// setting object shader
	SetShaderMaterial("plate");

	// draw the mesh with transformation values
	m_basicMeshes->DrawBoxMesh();



	//box mesh that acts as the bottom of the body for the holder (right side)
	scaleXYZ = glm::vec3(0.2f, 0.05f, 0.2f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = -125.0f;
	YrotationDegrees = -45.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3(0.93f, 1.35f, 1.8f);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//setting texture for the object
	SetShaderTexture("grayHolder");
	SetTextureUVScale(1.0, 1.0);

	// setting object shader
	SetShaderMaterial("plate");

	// draw the mesh with transformation values
	m_basicMeshes->DrawBoxMesh();


}

void SceneManager::RenderBook() {
	/*********************************************************************
	* This section will be covering the book that will be on the desk
	*
	* Will be using a box mesh to create the book
	*
	**********************************************************************/
	glm::vec3 scaleXYZ;
	float XrotationDegrees = 0.0f;
	float YrotationDegrees = 0.0f;
	float ZrotationDegrees = 0.0f;
	glm::vec3 positionXYZ;

	scaleXYZ = glm::vec3(3.0f, 0.5f, 2.0f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 45.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3(-7.0f, 0.25f, 1.8f);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//setting texture for the object
	SetShaderTexture("book");
	SetTextureUVScale(1.0, 1.0);

	// setting object shader
	SetShaderMaterial("book");

	// draw the mesh with transformation values
	m_basicMeshes->DrawBoxMesh();
}

void SceneManager::RenderWaterBottle() {


	/*********************************************************************
	* This section will cover the water bottle
	*
	* I will be using a cylinder mesh to create the water bottle
	* Then a pyramid mesh to create the top of the water bottle
	* And finally a smaller cylinder mesh to create the cap of the water bottle
	**********************************************************************/


	glm::vec3 scaleXYZ;
	float XrotationDegrees = 0.0f;
	float YrotationDegrees = 0.0f;
	float ZrotationDegrees = 0.0f;
	glm::vec3 positionXYZ;

	// Cylinder Mesh that will act as the body of the water bottle
	scaleXYZ = glm::vec3(1.0f, 2.5f, 1.0f);
	positionXYZ = glm::vec3(-2.5f, 0.0f, 1.8f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	SetShaderTexture("waterBottle");
	SetTextureUVScale(1.0, 1.0);
	SetShaderMaterial("glass");

	m_basicMeshes->DrawCylinderMesh();

	// Pyramid Mesh that will act as the top of the water bottle
	scaleXYZ = glm::vec3(1.0f, 1.4f, 1.0f);
	positionXYZ = glm::vec3(-2.5f, 2.5f, 1.8f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	SetShaderTexture("waterBottle");
	SetTextureUVScale(1.0, 1.0);
	SetShaderMaterial("glass");

	m_basicMeshes->DrawConeMesh();

	// Cylinder Mesh that will act as the cap of the water bottle
	scaleXYZ = glm::vec3(0.3f, 0.5f, 0.3f);
	positionXYZ = glm::vec3(-2.5f, 3.393f, 1.8f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	SetShaderTexture("waterBottle");
	SetTextureUVScale(1.0, 1.0);
	SetShaderMaterial("glass");

	m_basicMeshes->DrawCylinderMesh();
}

void SceneManager::RenderMonitors() {
	/*********************************************************************
	* This section will be covering the monitors that will be on the desk
	*
	* Will be using a box mesh to create the monitor
	*
	* One monitor on the left side of the objects
	* One monitor on the right side of the objects
	*
	**********************************************************************/


	glm::vec3 scaleXYZ;
	float XrotationDegrees = 0.0f;
	float YrotationDegrees = 0.0f;
	float ZrotationDegrees = 0.0f;
	glm::vec3 positionXYZ;

	// Monitor on the left side of the objects
	// Dimensions for a 32-inch monitor scaled down to 40%
	float monitorWidth = 27.9f * 0.4f;
	float monitorHeight = 15.7f * 0.4f;
	float monitorDepth = 2.0f * 0.4f;

	// Scale for the monitor
	scaleXYZ = glm::vec3(monitorWidth, monitorHeight, monitorDepth);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 15.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3(-8.0f, 6.0f, -4.0f);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	// setting texture for the object
	SetShaderTexture("keyboard");
	SetTextureUVScale(1.0, 1.0);

	// setting object shader
	SetShaderMaterial("plate");

	// draw the mesh with transformation values
	m_basicMeshes->DrawBoxMesh();



	// base of monitor on the left side of the objects

	// Scale for the base
	scaleXYZ = glm::vec3(4.0f, 0.2f, 4.0f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 15.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3(-8.0f, 0.2f, -4.5f);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	// setting texture for the object
	SetShaderTexture("keyboard");
	SetTextureUVScale(1.0, 1.0);

	// setting object shader
	SetShaderMaterial("plate");

	// draw the mesh with transformation values
	m_basicMeshes->DrawBoxMesh();

	// back handle of monitor on the left side of the objects

	// Scale for the handle
	scaleXYZ = glm::vec3(1.0f, 4.0f, 1.0f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 15.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3(-8.0f, 2.0f, -4.8f);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	// setting texture for the object
	SetShaderTexture("keyboard");
	SetTextureUVScale(1.0, 1.0);

	// setting object shader
	SetShaderMaterial("plate");

	// draw the mesh with transformation values
	m_basicMeshes->DrawBoxMesh();



	// Monitor on the right side of the objects
	// Dimensions for a 32-inch monitor scaled down to 40%
	monitorWidth = 27.9f * 0.4f;
	monitorHeight = 15.7f * 0.4f;
	monitorDepth = 2.0f * 0.4f;

	// Scale for the monitor
	scaleXYZ = glm::vec3(monitorWidth, monitorHeight, monitorDepth);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = -15.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3(8.0f, 6.0f, -4.0f);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);
	
	// setting texture for the object
	SetShaderTexture("keyboard");
	SetTextureUVScale(1.0, 1.0);

	// setting object shader
	SetShaderMaterial("plate");

	// draw the mesh with transformation values
	m_basicMeshes->DrawBoxMesh();

	// base of monitor on the right side of the objects

	// Scale for the base
	scaleXYZ = glm::vec3(4.0f, 0.2f, 4.0f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = -15.0f;
	ZrotationDegrees = 0.0f;
	
	// set the XYZ position for the mesh
	positionXYZ = glm::vec3(8.0f, 0.2f, -4.5f);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	// setting texture for the object
	SetShaderTexture("keyboard");
	SetTextureUVScale(1.0, 1.0);

	// setting object shader
	SetShaderMaterial("plate");

	// draw the mesh with transformation values
	m_basicMeshes->DrawBoxMesh();

	// back handle of monitor on the right side of the objects

	// Scale for the handle
	scaleXYZ = glm::vec3(1.0f, 4.0f, 1.0f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = -15.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3(8.0f, 2.0f, -4.8f);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	// setting texture for the object
	SetShaderTexture("keyboard");
	SetTextureUVScale(1.0, 1.0);

	// setting object shader
	SetShaderMaterial("plate");

	// draw the mesh with transformation values
	m_basicMeshes->DrawBoxMesh();





	// screen on right monitor
	// Scale for the screen
	scaleXYZ = glm::vec3(10.0f, 5.0f, 0.1f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = -15.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3(7.9f, 6.0f, -3.66f);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	SetShaderColor(1.0f, 1.0f, 1.0f, 1.0f);

	// draw the mesh with transformation values
	m_basicMeshes->DrawBoxMesh();


	// screen on left monitor
	// Scale for the screen
	scaleXYZ = glm::vec3(10.0f, 5.0f, 0.1f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 15.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3(-7.9f, 6.0f, -3.66f);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	SetShaderColor(1.0f, 1.0f, 1.0f, 1.0f);

	// draw the mesh with transformation values
	m_basicMeshes->DrawBoxMesh();

}

void SceneManager::RenderKeyBoard() {
	/*********************************************************************
	* This section will be covering the keyboard that will be on the desk
	*
	* Will be using box meshes to create the keyboard base and keys
	*
	**********************************************************************/
	
	glm::vec3 scaleXYZ;
	float XrotationDegrees = 0.0f;
	float YrotationDegrees = 0.0f;
	float ZrotationDegrees = 0.0f;
	glm::vec3 positionXYZ;

	// Keyboard base

	// Scale for the keyboard base
	scaleXYZ = glm::vec3(10.2f, 0.5f, 4.0f); 

	// Position on the desk
	positionXYZ = glm::vec3(0.1f, 0.25f, 6.0f); 

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	// Set the texture for the object
	SetShaderTexture("keyboard");
	SetTextureUVScale(1.0f, 1.0f);

	// Set the object material into the shader
	SetShaderMaterial("plate");

	m_basicMeshes->DrawBoxMesh();

	// Keys

	// Scale for the keys
	float keyWidth = 0.8f;
	float keyHeight = 0.2f;
	float keyDepth = 0.8f;

	// Spacing and number of rows and columns for the keys
	float spacing = 0.05f; 
	int numRows = 5;
	int numCols = 12;

	// Start position for the keys
	float startX = -5.0f + keyWidth / 2.0f;
	float startY = 0.2f + 0.25f + keyHeight / 2.0f;
	float startZ = 8.0f - keyDepth / 2.0f;

	// Using for loop to create each key that will go on top of the keyboard base
	for (int row = 0; row < numRows; ++row) {
		for (int col = 0; col < numCols; ++col) {
			scaleXYZ = glm::vec3(keyWidth, keyHeight, keyDepth);
			positionXYZ = glm::vec3(startX + col * (keyWidth + spacing), startY, startZ - row * (keyDepth + spacing));

			SetTransformations(
				scaleXYZ,
				XrotationDegrees,
				YrotationDegrees,
				ZrotationDegrees,
				positionXYZ);

			// Set the texture for the object
			SetShaderTexture("body");
			SetTextureUVScale(1.0, 1.0);

			// Set the object material into the shader
			SetShaderMaterial("plate");

			m_basicMeshes->DrawBoxMesh();
		}
	}
}

void SceneManager::RenderMouse() {
	/*********************************************************************
	* This section will be covering the mouse that will be on the desk
	*
	* Will be using a cylinder and half-sphere meshes to create the mouse
	*
	**********************************************************************/

	glm::vec3 scaleXYZ;
	float XrotationDegrees = 0.0f;
	float YrotationDegrees = -45.0f;
	float ZrotationDegrees = 0.0f;
	glm::vec3 positionXYZ;

	// Mouse base (cylinder)

	// Scale for the cylinder
	scaleXYZ = glm::vec3(1.5f, 0.5f, 1.0f); 

	// Position on the desk
	positionXYZ = glm::vec3(7.0f, 0.25f, 6.0f);

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	// Set the texture for the object
	SetShaderTexture("mouse");
	SetTextureUVScale(1.0, 1.0);

	// Set the object material into the shader
	SetShaderMaterial("plate");

	// Drawing a cylinder without the top and bottom faces
	m_basicMeshes->DrawCylinderMesh(false, true, true); 

	// Mouse top (half-sphere)

	// Scale for the half-sphere

	scaleXYZ = glm::vec3(1.5f, 0.5f, 1.0f); 

	// Position on the desk
	positionXYZ = glm::vec3(7.0f, 0.75f, 6.0f); 

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	// Set the texture for the object
	SetShaderTexture("mouse");
	SetTextureUVScale(1.0, 1.0);

	// Set the object material into the shader
	SetShaderMaterial("plate");

	m_basicMeshes->DrawHalfSphereMesh(); // Draw half-sphere
}








	
