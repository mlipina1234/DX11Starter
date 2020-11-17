#include "Entity.h"

Mesh* Entity::GetMesh()
{
	return mesh;
}

Transform* Entity::GetTransform()
{
	return &transform;
}

Entity::Entity(Mesh* mesh, Material* material)
{
	this->mesh = mesh;
	this->material = material;
	transform = Transform();
}

void Entity::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context,  Camera* cam)
{
	material->GetVertexShader()->SetShader(); 
	material->GetPixelShader()->SetShader();
	SimpleVertexShader* vs = material->GetVertexShader(); // Simplifies next few lines
	vs->SetFloat4("colorTint", material->GetColorTint());
	vs->SetMatrix4x4("world", transform.GetWorldMatrix());
	vs->SetMatrix4x4("view", cam->GetViewMatrix());
	vs->SetMatrix4x4("proj", cam->GetProjectionMatrix());
	vs->CopyAllBufferData();

	SimplePixelShader* ps = material->GetPixelShader(); // Simplifies next few lines
	ps->SetFloat3("cameraPosition", cam->GetTransform()->GetPosition());
	ps->SetFloat("specularValue", material->GetSpecularity());
	ps->SetSamplerState("samplerOptions", material->GetSamplerState());
	ps->SetShaderResourceView("Albedo", material->GetSRV());
	ps->SetShaderResourceView("RoughnessMap", material->GetRoughnessSRV());
	ps->SetShaderResourceView("MetalnessMap", material->GetMetalnessSRV());
	if (material->GetNormalSRV() != nullptr)
	{
		ps->SetShaderResourceView("NormalMap", material->GetNormalSRV());
	}
	ps->CopyAllBufferData();


	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	// Set buffers in the input assembler
		//  - Do this ONCE PER OBJECT you're drawing, since each object might
		//    have different geometry.
		//  - for this demo, this step *could* simply be done once during Init(),
		//    but I'm doing it here because it's often done multiple times per frame
		//    in a larger application/game
	context->IASetVertexBuffers(0, 1, GetMesh()->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(mesh->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	// Finally do the actual drawing
	//  - Do this ONCE PER OBJECT you intend to draw
	//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
	//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
	//     vertices in the currently set VERTEX BUFFER
	context->DrawIndexed(
		mesh->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices
}