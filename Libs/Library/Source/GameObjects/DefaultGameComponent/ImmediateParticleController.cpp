#pragma once
#include "stdafx.h"
#include"../../../Header/Resources/Resource_RealTimeMesh.h"

void ButiEngine::ImmediateParticleController::OnUpdate()
{
	auto endItr = vec_particleInfo.end();
	int i = 0;
	int remove = 0;
	for (auto itr = vec_particleInfo.begin(); itr != endItr; i++) {
		itr->position += itr->velocity* GameDevice::WorldSpeed + itr->force * GameDevice::WorldSpeed;
		itr->force *= itr->accelation * GameDevice::WorldSpeed;
		itr->color += itr->colorPase * GameDevice::WorldSpeed;
		itr->size += itr->sizePase * GameDevice::WorldSpeed;
		itr->angle += itr->anglePase * GameDevice::WorldSpeed;
		itr->life-=1.0f * GameDevice::WorldSpeed;

		shp_backUp->vertices.at(i).position = itr->position;
		shp_backUp->vertices.at(i).uv.x = itr->size;
		shp_backUp->vertices.at(i).color.x = itr->axis.x;
		shp_backUp->vertices.at(i).color.y = itr->axis.y;
		shp_backUp->vertices.at(i).color.z = itr->axis.z;
		shp_backUp->vertices.at(i).color.w= itr->angle;
		shp_backUp->vertices.at(i).normal = itr->color;

		if (itr->life <= 0.0f) {
			itr = vec_particleInfo.erase(itr);
			endItr = vec_particleInfo.end();
			remove++;
		}else{
			itr++;
		}
	}
	remove - addParticleCount;
	for (int j = 0; j < remove; j++) {
		shp_backUp->vertices.at(i + j).uv.x = 0.0f;
	}

	addParticleCount=0;
	shp_mesh->Update();
}

void ButiEngine::ImmediateParticleController::OnSet()
{
	meshTag = gameObject.lock()->GetResourceContainer()->GetMeshTag("Particle");
	shp_mesh = gameObject.lock()->GetResourceContainer()->GetMesh(meshTag).lock()->GetThis<Resource_RealTimeMesh>();
	shp_backUp = shp_mesh->GetBackupData_Row()->GetThis<BackupData<Vertex::Vertex_UV_Normal_Color>>();
}

void ButiEngine::ImmediateParticleController::Start()
{
	auto endItr = shp_backUp->vertices.end();
	for (auto itr = shp_backUp->vertices.begin(); itr != endItr; itr++) {
		*itr = Vertex::Vertex_UV_Normal_Color();
	}
}

void ButiEngine::ImmediateParticleController::AddParticle(const Particle3D& arg_particle)
{
	if (vec_particleInfo.size() + 1 > shp_mesh->GetVertexCount()) {
		return;
	}
	vec_particleInfo.push_back(arg_particle);
	addParticleCount++;
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::ImmediateParticleController::Clone()
{
	auto ret= ObjectFactory::Create<ImmediateParticleController>();
	ret->meshTag = meshTag;
	return ret;
}

void ButiEngine::ImmediateParticleController::OnRemove()
{
	auto endItr = shp_backUp->vertices.end();
	for (auto itr= shp_backUp->vertices.begin(); itr!=endItr; itr++) {
		itr->uv.x = 0;
	}
}

