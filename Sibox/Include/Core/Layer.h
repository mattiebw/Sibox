#pragma once

class Layer
{
public:
	virtual void OnAttach();
	virtual void OnDetach();
	virtual void Update(f64 delta);
	virtual void Render(f64 delta);
	virtual void RenderImGUI(f64 delta);
};
