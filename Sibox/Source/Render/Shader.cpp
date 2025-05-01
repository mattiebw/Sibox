#include "siboxpch.h"
#include "Render/Shader.h"

Shader::Shader(std::string name)
	: m_Name(std::move(name))
{
	m_ProgramID = glCreateProgram();
}

Shader::~Shader()
{
	CleanUp();
}

s32 Shader::AddStageFromSource(GLenum stage, std::string_view source)
{
	if (m_IsComplete)
	{
		SIBOX_ERROR("In shader \"{0}\", attempting to add a stage to a linked shader!", m_Name);
		return -1;
	}

	if (!IsValidShaderStage(stage))
	{
		SIBOX_ERROR("Shader::AddStage of shader \"{0}\" called with invalid stage enum: {1}.\nShould be one of: "
		            "GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_TESS_EVALUATION_SHADER, GL_TESS_CONTROL_SHADER, GL_COMPUTE_SHADER",
		            m_Name, stage);
		return -1;
	}

	for (u32 i = 0; i < m_UsedVertexAttributes; i++)
		glEnableVertexAttribArray(i);

	s32         shaderID = glCreateShader(stage);
	s32         length   = static_cast<s32>(source.length());
	const char *data     = source.data();

	// Check for version, and remove any BOM that can break the shader on Linux.
	while (length > 0 && *data != '#')
	{
		data++;
		length--;
	}

	if (length == 0)
	{
		SIBOX_ERROR(
			"Shader::AddStage of shader \"{0}\" called with invalid source: {1}. The source file did not begin with a #version.",
			m_Name, source);
		return -1;
	}

	glShaderSource(shaderID, 1, &data, &length);
	glCompileShader(shaderID);

	GLint compiled;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);
	if (compiled != GL_TRUE)
	{
		GLsizei       logLength = 0;
		static GLchar message[1024];
		glGetShaderInfoLog(shaderID, 1024, &logLength, message);
		SIBOX_ERROR_NO_NEWLINE("In shader {0}, failed to compile {1}: {2}", m_Name, GetShaderTypeString(stage),
		                       std::string_view(&message[0], logLength));
		glDeleteShader(shaderID);
		return -1;
	}

	m_ShaderStages.emplace_back(shaderID);

	return shaderID;
}

s32 Shader::AddStageFromFile(GLenum stage, std::string_view source)
{
	if (m_IsComplete)
	{
		SIBOX_ERROR("In shader \"{0}\", attempting to add a stage to a linked shader!", m_Name);
		return -1;
	}

	// Open source file and read to string stream
	std::ifstream sourceFile(source.data());
	if (!sourceFile.good())
	{
		SIBOX_ERROR("Failed to open shader source \"{0}\"", source);
		return -1;
	}
	std::stringstream buffer;
	buffer << sourceFile.rdbuf();

	// Call into add stage from source
	const s32 result = AddStageFromSource(stage, buffer.str());

	// Clean up
	sourceFile.close();

	return result;
}

s32 Shader::LinkProgram()
{
	BindAttributes();

	for (u32 i = 0; i < m_UsedVertexAttributes; i++)
		glEnableVertexAttribArray(i);

	// Create the program and attach and link all shaders
	for (const s32 shaderID : m_ShaderStages)
		glAttachShader(m_ProgramID, shaderID);
	glLinkProgram(m_ProgramID);
	glValidateProgram(m_ProgramID);

	// Check we linked successfully
	s32 success = 0;
	glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		// Uh oh, we failed to link. Let's print out a reason.
		m_HasError = true;

		// Get error log and print it
		GLint logLength = 0;
		glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> infoLog(logLength);
		glGetProgramInfoLog(m_ProgramID, logLength, &logLength, infoLog.data());
		if (logLength != 0)
			SIBOX_ERROR("Failed to link shader {0}: {1}", m_Name, infoLog.data());
		else
			SIBOX_ERROR("Shader \"{}\" failed to compile, but OpenGL didn't provide an error.", m_Name);

		// Clean up
		for (const s32 shaderID : m_ShaderStages)
		{
			glDetachShader(m_ProgramID, shaderID);
			glDeleteShader(shaderID);
		}

		return -1;
	}

	SIBOX_INFO("Created shader program {0}!", m_Name);

	// Clean up by detaching
	for (const s32 shaderID : m_ShaderStages)
	{
		glDetachShader(m_ProgramID, shaderID);
		glDeleteShader(shaderID);
	}
	m_ShaderStages.clear();

	GetUniformLocations();

	m_IsComplete = true;

	return m_ProgramID;
}

void Shader::Bind() const
{
	SIBOX_ASSERT(m_ProgramID != 0 && "Attempting to bind to invalid shader (invalid program ID)");
	glUseProgram(m_ProgramID);
	if (m_TwoSided)
		glDisable(GL_CULL_FACE);
	else
		glEnable(GL_CULL_FACE);
}

void Shader::BindAttributes()
{
	// This function can be overridden by child shader programs to bind attributes
}

void Shader::BindAttribute(int attribute, std::string_view variableName)
{
	glBindAttribLocation(m_ProgramID, attribute, variableName.data());
	m_UsedVertexAttributes = std::max(m_UsedVertexAttributes, static_cast<u32>(attribute + 1));
}

void Shader::CleanUp()
{
	// if (Renderer::GetBoundShader() == this)
	// 	Renderer::BindShader(nullptr);

	glDeleteProgram(m_ProgramID);
	m_ProgramID = 0;
}

const char* Shader::GetShaderTypeString(GLenum type)
{
	switch (type)
	{
	case GL_VERTEX_SHADER:
		return "Vertex Shader";
	case GL_FRAGMENT_SHADER:
		return "Fragment Shader";
	case GL_GEOMETRY_SHADER:
		return "Geometry Shader";
	case GL_TESS_CONTROL_SHADER:
		return "Tessellation Control Shader";
	case GL_TESS_EVALUATION_SHADER:
		return "Tessellation Evaluation Shader";
	case GL_COMPUTE_SHADER:
		return "Compute Shader";
	default:
		return "Unknown Shader Type"; // Dynamic string doesn't work here, either pointer to freed or memory leak.
	// return fmt::format("Unknown Shader Type ({0})", type).c_str();
	}
}

// MW @todo: Here we have manual uniform location caching.
// There are GL functions to get all uniform locations, get their names, and then cache them like that, without
// needing to query each one individually.

int Shader::GetUniformLocation(std::string_view uniformName)
{
	auto pos = m_UniformLocations.find(uniformName.data());
	if (pos == m_UniformLocations.end())
	{
		int location = glGetUniformLocation(m_ProgramID, uniformName.data());
		if (location == -1)
			SIBOX_WARN("Uniform \"{0}\" not found in shader \"{1}\"", uniformName, m_Name);
		m_UniformLocations[std::string(uniformName)] = location;
		return location;
	}
	return pos->second;
}

int Shader::GetUniformLocation(std::string_view uniformName) const
{
	auto pos = m_UniformLocations.find(uniformName.data());
	if (pos == m_UniformLocations.end())
	{
		int location = glGetUniformLocation(m_ProgramID, uniformName.data());
		if (location == -1)
			SIBOX_WARN("Uniform \"{0}\" not found in shader \"{1}\"", uniformName, m_Name);
		return location;
	}
	return pos->second;
}

void Shader::GetUniformLocations()
{
}

void Shader::SetUniform1i(std::string_view uniformName, int value) const
{
	glUniform1i(GetUniformLocation(uniformName), value);
}

void Shader::SetUniform1f(std::string_view uniformName, f32 value) const
{
	glUniform1f(GetUniformLocation(uniformName), value);
}

void Shader::SetUniform2f(std::string_view uniformName, f32 x, f32 y) const
{
	glUniform2f(GetUniformLocation(uniformName), x, y);
}

void Shader::SetUniform2i(std::string_view uniformName, int x, int y) const
{
	glUniform2i(GetUniformLocation(uniformName), x, y);
}

void Shader::SetUniformVec2(std::string_view uniformName, const Vector2F &vec) const
{
	glUniform2f(GetUniformLocation(uniformName), vec.X, vec.Y);
}

void Shader::SetUniformIVec2(std::string_view uniformName, const Vector2I &vec) const
{
	glUniform2i(GetUniformLocation(uniformName), vec.X, vec.Y);
}

void Shader::SetUniform3f(std::string_view uniformName, f32 x, f32 y, float z) const
{
	glUniform3f(GetUniformLocation(uniformName), x, y, z);
}

void Shader::SetUniform3i(std::string_view uniformName, int x, int y, int z) const
{
	glUniform3i(GetUniformLocation(uniformName), x, y, z);
}

void Shader::SetUniformVec3(std::string_view uniformName, const Vector3F &vec) const
{
	glUniform3f(GetUniformLocation(uniformName), vec.X, vec.Y, vec.Z);
}

void Shader::SetUniformIVec3(std::string_view uniformName, const Vector3I &vec) const
{
	glUniform3i(GetUniformLocation(uniformName), vec.X, vec.Y, vec.Z);
}

void Shader::SetUniform4f(std::string_view uniformName, float x, float y, float z, float w) const
{
	glUniform4f(GetUniformLocation(uniformName), x, y, z, w);
}

void Shader::SetUniformVec4(std::string_view uniformName, const Vector4F &vec) const
{
	glUniform4f(GetUniformLocation(uniformName), vec.X, vec.Y, vec.Z, vec.W);
}

void Shader::SetUniformMatrix3f(std::string_view uniformName, const Matrix3x3F &matrix) const
{
	glUniformMatrix3fv(GetUniformLocation(uniformName), 1, GL_FALSE, matrix.Data());
}

void Shader::SetUniformMatrix4f(std::string_view uniformName, const Matrix4x4F &matrix) const
{
	glUniformMatrix4fv(GetUniformLocation(uniformName), 1, GL_FALSE, matrix.Data());
}

void Shader::SetTwoSided(bool twoSided)
{
	m_TwoSided = twoSided;

	// if (Renderer::GetBoundShader() == this)
	// {
	// 	if (m_TwoSided)
	// 		glDisable(GL_CULL_FACE);
	// 	else
	// 		glEnable(GL_CULL_FACE);
	// }
}

void Shader::SetUniform1b(std::string_view uniformName, bool value) const
{
	glUniform1i(GetUniformLocation(uniformName), value ? 1 : 0);
}
