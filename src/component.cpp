#include "component.h"
#include "gameObject.h"

void DefaultRenderingEngine::Render(GameObject* pGameObject)
{
    for(int i = 0; i < pGameObject->GetNumberOfChildren(); i++)
    {
        GameObject* pChild = pGameObject->GetChild(i);
        
        pChild->GetTransform().SetChildModel(pGameObject->GetTransform().GetModel(false));
        pChild->Render();
        Render(pChild);
    }
}

