#pragma once

class Shader
{
public:
	Shader(std::string name);
	virtual ~Shader();
	Shader(Shader &&other) noexcept            = delete; // Disallow move constructor
	Shader(const Shader &other)                = delete; // Disallow copy constructor
	Shader& operator=(const Shader &other)     = delete; // Disallow copy assignment
	Shader& operator=(Shader &&other) noexcept = delete; // Disallow move assignment

	static FORCEINLINE Ref<Shader> CreateShaderProgram(std::string_view name)
	{
		return CreateRef<Shader>(std::string(name));
	}

	int32_t AddStageFromSource(GLenum stage, std::string_view source);
	int32_t AddStageFromFile(GLenum stage, std::string_view source);
	int32_t LinkProgram();

	// Binding functions
	void Bind() const;

	static FORCEINLINE void StaticUnbind()
	{
		glUseProgram(0);
	}

	// Uniform functions
	int               GetUniformLocation(std::string_view uniformName);
	[[nodiscard]] int GetUniformLocation(std::string_view uniformName) const;
	virtual void      GetUniformLocations();

	void SetUniform1b(std::string_view uniformName, bool value) const;
	void SetUniform1i(std::string_view uniformName, int value) const;
	void SetUniform1f(std::string_view uniformName, float value) const;
	void SetUniform2f(std::string_view uniformName, float x, float y) const;
	void SetUniform2i(std::string_view uniformName, int x, int y) const;
	void SetUniformVec2(std::string_view uniformName, const glm::vec2 &vec) const;
	void SetUniformIVec2(std::string_view uniformName, const glm::ivec2 &vec) const;
	void SetUniform3f(std::string_view uniformName, float x, float y, float z) const;
	void SetUniform3i(std::string_view uniformName, int x, int y, int z) const;
	void SetUniformVec3(std::string_view uniformName, const glm::vec3 &vec) const;
	void SetUniformIVec3(std::string_view uniformName, const glm::ivec3 &vec) const;
	void SetUniform4f(std::string_view uniformName, float x, float y, float z, float w) const;
	void SetUniformVec4(std::string_view uniformName, const glm::vec4 &vec) const;
	void SetUniformMatrix3f(std::string_view uniformName, const glm::mat3 &vec) const;
	void SetUniformMatrix4f(std::string_view uniformName, const glm::mat4 &matrix) const;

	[[nodiscard]] FORCEINLINE bool IsTwoSided() const { return m_TwoSided; }
	void                           SetTwoSided(bool twoSided);

	FORCEINLINE bool IsComplete() const { return m_IsComplete; }
	FORCEINLINE bool HasError() const { return m_HasError; }

	NODISCARD FORCEINLINE std::string GetName() const { return m_Name; }

	virtual void BindAttributes();
	void         BindAttribute(int attribute, std::string_view variableName);

	void CleanUp();

	static FORCEINLINE bool IsValidShaderStage(GLenum value)
	{
		return value == GL_VERTEX_SHADER || value == GL_FRAGMENT_SHADER || value == GL_GEOMETRY_SHADER || value ==
			GL_TESS_EVALUATION_SHADER || value == GL_TESS_CONTROL_SHADER || value == GL_COMPUTE_SHADER;
	}

	static const char* GetShaderTypeString(GLenum type);

protected:
	std::vector<int32_t>                     m_ShaderStages;
	std::unordered_map<std::string, int32_t> m_UniformLocations;
	uint32_t                                 m_UsedVertexAttributes = 0;

	std::string m_Name       = "Untitled Shader";
	int32_t     m_ProgramID  = -1;
	bool        m_IsComplete = false, m_HasError = false;

	bool m_TwoSided = false;
};
