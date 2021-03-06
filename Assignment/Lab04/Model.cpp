#include "Model.h"

Model::Model(const char* mesh_name, vec3 starting_pos)
{
	model_data = load_mesh(mesh_name);
	pos = starting_pos;
	isMoving = false;
}

Model::Model(const char* mesh_name, vec3 starting_pos, bool isComputer) : Model(mesh_name, starting_pos)
{
	isAI = isComputer;
	isMoving = true;
	vel = vec3(0.0f, 0.0f, 0.05f);
}

void Model::update()
{
	if (isAI)
	{
		pos -= vel;
		if (pos.v[2] <= -100.0f)
		{
			pos.v[2] = 100.0f;
		}
	}
	else
	{
		vec3 new_vel = vel;

		if (isMoving) {
			new_vel.v[0] = vel.v[0] * sinf(rot.v[1] * M_PI / 180.0f);
			new_vel.v[2] = vel.v[2] * cosf(rot.v[1] * M_PI / 180.0f);
		}

		pos += new_vel;
		rot += rot_vel;
	}
}

#pragma region MESH LOADING
/*----------------------------------------------------------------------------
MESH LOADING FUNCTION
----------------------------------------------------------------------------*/

ModelData Model::load_mesh(const char* file_name) {
	ModelData modelData;

	/* Use assimp to read the model file, forcing it to be read as    */
	/* triangles. The second flag (aiProcess_PreTransformVertices) is */
	/* relevant if there are multiple meshes in the model file that   */
	/* are offset from the origin. This is pre-transform them so      */
	/* they're in the right position.                                 */
	const aiScene* scene = aiImportFile(
		file_name,
		aiProcess_Triangulate | aiProcess_PreTransformVertices
	);

	if (!scene) {
		fprintf(stderr, "ERROR: reading mesh %s\n", file_name);
		return modelData;
	}

	printf("  %i materials\n", scene->mNumMaterials);
	printf("  %i meshes\n", scene->mNumMeshes);
	printf("  %i textures\n", scene->mNumTextures);

	min_x = max_x = scene->mMeshes[0]->mVertices[0].x;
	min_y = max_y = scene->mMeshes[0]->mVertices[0].y;
	min_z = max_z = scene->mMeshes[0]->mVertices[0].z;
	
	for (unsigned int m_i = 0; m_i < scene->mNumMeshes; m_i++) {
		const aiMesh* mesh = scene->mMeshes[m_i];
		printf("    %i vertices in mesh\n", mesh->mNumVertices);
		modelData.mPointCount += mesh->mNumVertices;
		for (unsigned int v_i = 0; v_i < mesh->mNumVertices; v_i++) {
			if (mesh->HasPositions()) {
				const aiVector3D* vp = &(mesh->mVertices[v_i]);
				modelData.mVertices.push_back(vec3(vp->x, vp->y, vp->z));

				if (vp->x < min_x)
					min_x = vp->x;
				if (vp->x > max_x)
					max_x = vp->x;
				if (vp->y < min_y)
					min_y = vp->y;
				if (vp->y > max_y)
					max_y = vp->y;
				if (vp->z < min_z)
					min_z = vp->z;
				if (vp->z > max_z)
					max_z = vp->z;
			}
			if (mesh->HasNormals()) {
				const aiVector3D* vn = &(mesh->mNormals[v_i]);
				modelData.mNormals.push_back(vec3(vn->x, vn->y, vn->z));
			}
			if (mesh->HasTextureCoords(0)) {
				const aiVector3D* vt = &(mesh->mTextureCoords[0][v_i]);
				modelData.mTextureCoords.push_back(vec2(vt->x, vt->y));
			}
			if (mesh->HasTangentsAndBitangents()) {
				/* You can extract tangents and bitangents here              */
				/* Note that you might need to make Assimp generate this     */
				/* data for you. Take a look at the flags that aiImportFile  */
				/* can take.                                                 */
			}
		}
	}

	min_vec = vec4(min_x, min_y, min_z, 1);
	max_vec = vec4(max_x, max_y, max_z, 1);

	aiReleaseImport(scene);
	return modelData;
}
#pragma endregion MESH LOADING