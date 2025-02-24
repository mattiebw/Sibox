#pragma once

class Layer
{
public:
	virtual void OnAttach();
	virtual void OnDetach();
	virtual void Update(double delta);
	virtual void Render(double delta);
	virtual void RenderImGUI(double delta);
};
