#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleResource.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "GameObject.h"
#include "ModuleMeshes.h"
#include "ModuleGUI.h"
#include "ModuleFileSystem.h"
#include "ModuleWindow.h"

#include "Resource.h"
#include "ResourceMesh.h"

ComponentMesh::ComponentMesh(GameObject* parent, componentType type) : Component(parent, type)
{

}

ComponentMesh::~ComponentMesh()
{
}

bool ComponentMesh::Update()
{
	ResourceMesh* mesh =(ResourceMesh*) App->resources->GetResource(RUID);

	if (!active || mesh == nullptr )
		return false;

	//Assign Vertices
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
	glVertexPointer(3, GL_FLOAT, 0, &mesh->vertices[0]);

	//Assign texture
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	if (mesh->texCoords != nullptr)
		glTexCoordPointer(2, GL_FLOAT, 0, &mesh->texCoords[0]);

	//Draw
	glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);

	//Disable
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	if (App->renderer3D->Normals && mesh->normals != nullptr)
	{
		glLineWidth(2.0f);
		glColor3f(0, 0.5f, 1);

		glBegin(GL_LINES);
		for (int i = 0; i < mesh->num_vertices * 3; i = i + 3)
		{
			glVertex3f(mesh->vertices[i], mesh->vertices[i + 1], mesh->vertices[i + 2]);
			glVertex3f(-mesh->normals[i] * 3 + mesh->vertices[i], -mesh->normals[i + 1] * 3 + mesh->vertices[i + 1], -mesh->normals[i + 2] * 3 + mesh->vertices[i + 2]);
		}
		glEnd();

		glColor3f(1, 1, 1);
		glLineWidth(1.0f);
	}

	if (App->renderer3D->Faces && mesh->faceNormals.size() > 0)
	{
		int vert_normal = 0;

		glLineWidth(2.0f);
		glColor3f(0, 0.5f, 1);

		glBegin(GL_LINES);
		for (int i = 0; i < mesh->faceNormals.size(); i = i + 6)
		{
			glVertex3f(mesh->faceNormals[i], mesh->faceNormals[i + 1], mesh->faceNormals[i + 2]);
			glVertex3f(mesh->faceNormals[i + 3] + mesh->faceNormals[i], mesh->faceNormals[i + 4] + mesh->faceNormals[i + 1], mesh->faceNormals[i + 5] + mesh->faceNormals[i + 2]);
			vert_normal += 9;
		}
		glEnd();

		glColor3f(1, 1, 1);
		glLineWidth(2.0f);
	}

	return true;
}

void ComponentMesh::DrawInfo()
{
	ResourceMesh* mesh = (ResourceMesh*)App->resources->GetResource(RUID);

	ImGui::PushID("toggleMesh");
	ImGui::Checkbox("", &active);
	ImGui::PopID();
	ImGui::SameLine();
	
	if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick))
	{
		std::string filename;
		std::string extension;
		if (mesh != nullptr)
			App->fileSystem->splitPath(mesh->GetFile(), nullptr, &filename, &extension);
		beginDroppableSpace((mesh == nullptr) ? "No Mesh" : (filename + '.' + extension).c_str(), mesh == nullptr);
		if (mesh != nullptr)
		{
			ImGui::Text("Triangles Count: %d", mesh->num_indices / 3);
			ImGui::Text("Vertices Count: %d", mesh->num_vertices);
			ImGui::Text("Mesh size:\n X: %f | Y: %f | Z: %f", mesh->boundingBox.Size().x, mesh->boundingBox.Size().y, mesh->boundingBox.Size().z);
		}
		else
		{
			ImGui::SameLine();
			ImGui::PushID("pickMesh");
			if (ImGui::RadioButton("", false))
			{
				ImVec2 pos = ImGui::GetWindowPos();
				if (pos.x < App->window->width / 2)
					pos.x += ImGui::GetWindowWidth();
				else
					pos.x -= 190;
				App->gui->startResourceList(R_MESH, pos.x, pos.y, this);
			}
			ImGui::PopID();
		}
	}
}

void ComponentMesh::Save(JSON_Value * component) const
{
	ResourceMesh* rMesh = (ResourceMesh*)App->resources->GetResource(RUID);

	JSON_Value* mesh = component->createValue();

	mesh->addInt("Type", type);
	mesh->addUint("UID",UID);
	mesh->addString("FBX", rMesh->GetFile());
	mesh->addString("mesh", rMesh->GetName());

	component->addValue("", mesh);
}

void ComponentMesh::Load(JSON_Value* component)
{
	UID = component->getUint("UID");
	RUID = App->resources->getResourceUIDFromMeta(component->getString("FBX"),component->getString("mesh"));

	App->resources->GetResource(RUID)->LoadtoMemory();

}
