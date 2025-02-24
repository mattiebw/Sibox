#include "papipch.h"
#include "Render/ShaderLibrary.h"

#include "Render/Shader.h"

std::unordered_map<std::string, Ref<Shader>> ShaderLibrary::s_Shaders;

Ref<Shader> ShaderLibrary::CreateShader(std::string_view name)
{
	std::string newName = std::string(name);
	Ref<Shader> shader  = CreateRef<Shader>(newName);
	AddShader(shader, newName);
	return shader;
}

Ref<Shader> ShaderLibrary::GetShader(std::string_view name)
{
	auto it = s_Shaders.find(name.data());
	if (it == s_Shaders.end())
	{
		PAPI_WARN("Shader \"{0}\" not found in library", name);
		return nullptr;
	}
	return it->second;
}

void ShaderLibrary::AddShader(const Ref<Shader> &shader)
{
	AddShader(shader, shader->GetName());
}

void ShaderLibrary::AddShader(const Ref<Shader> &shader, const std::string &name)
{
	PAPI_ASSERT(!ShaderExists(name) && "Shader with name already exists in library");
	s_Shaders[name] = shader;
}

bool ShaderLibrary::RemoveShader(std::string_view name)
{
	auto it = s_Shaders.find(name.data());
	if (it == s_Shaders.end())
	{
		PAPI_WARN("Shader \"{0}\" not found in library", name);
		return false;
	}
	s_Shaders.erase(it);
	return true;
}

void ShaderLibrary::Clear()
{
	s_Shaders.clear();
}
