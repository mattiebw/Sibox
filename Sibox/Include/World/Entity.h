#pragma once

class World;

// This macro is used to define the TypeID of an entity class, and also to inherit constructors.
// While "typeName::" is redundant, it will cause a compile error if the classname provided is not at least an Entity, so it can help if it was wrong.
#define ENTITY_CLASS_BODY(typeName) public: using Entity::Entity;\
    static constexpr uint16_t s_EntityTypeID = crc16(#typeName);\
    NODISCARD virtual uint16_t GetTypeID() const override { return typeName::s_EntityTypeID; }

class Entity
{
public:
	Entity();
	Entity(std::string name, UUID uuid = UUID());
	Entity(std::string name, const Transform &tf, UUID uuid = UUID());
	Entity(std::string name, Vector3F pos, Vector3F rot, Vector3F scale, UUID uuid = UUID());
	virtual ~Entity();

	Entity(const Entity &other)                = delete;
	Entity(Entity &&other) noexcept            = delete;
	Entity& operator=(const Entity &other)     = delete;
	Entity& operator=(Entity &&other) noexcept = delete;

	static Ref<Entity> CreateEntity(uint16_t type, std::string name, const Transform &tf, UUID uuid);
	static Ref<Entity> CreateEntity(uint16_t type, std::string name, UUID uuid);

	NODISCARD FORCEINLINE const std::string& GetName() const { return m_Name; }
	NODISCARD FORCEINLINE const Vector3F&   GetPosition() const { return EntityTransform.Position; }
	NODISCARD FORCEINLINE const Vector3F&   GetRotation() const { return EntityTransform.Rotation; }
	NODISCARD FORCEINLINE const Vector3F&   GetScale() const { return EntityTransform.Scale; }
	NODISCARD FORCEINLINE World*             GetWorld() const { return m_World; }
	NODISCARD FORCEINLINE UUID               GetUUID() const { return m_UUID; }
	NODISCARD FORCEINLINE EntityNetworkType  GetEntityNetworkType() const { return m_EntityNetworkType; }

	FORCEINLINE void SetName(const std::string &name) { m_Name = name; }
	FORCEINLINE void SetPosition(Vector3F newPos) { EntityTransform.Position = newPos; }
	FORCEINLINE void SetRotation(Vector3F newRot) { EntityTransform.Rotation = newRot; }
	FORCEINLINE void SetScale(Vector3F newScale) { EntityTransform.Scale = newScale; }
	FORCEINLINE void SetWorld(World *world) { m_World = world; }

	void SetUUID(UUID uuid);

	virtual void SendEvent(uint16_t eventID, Buffer eventData);
	virtual void ReceiveEntityEvent(uint16_t eventID, Buffer eventData);

	virtual void Created();
	virtual void AddedToWorld(World *world);
	virtual void Tick(double delta);
	virtual void Render();
	virtual void Destroyed();

	NODISCARD FORCEINLINE bool HasAuthority() { return m_EntityNetworkType != EntityNetworkType::RemoteProxy; }
	
	void Destroy();

	Transform EntityTransform;

	static constexpr uint16_t  s_EntityTypeID = crc16("Entity");
	NODISCARD virtual uint16_t GetTypeID() const { return s_EntityTypeID; };

protected:
	UUID        m_UUID;
	std::string m_Name  = "Entity";
	World *     m_World = nullptr;

	EntityNetworkType m_EntityNetworkType;

	friend class World;
};
