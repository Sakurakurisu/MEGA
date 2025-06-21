#pragma once  

#include "BaseManager.h"  
#include <vector>  

class Rigidbody2D;  

class RigidbodyMgr : public BaseManager<RigidbodyMgr>  
{  
friend class BaseManager<RigidbodyMgr>;  

public:  
// Rigidbody2Dを登録する  
void RegisterRigidbody2D(Rigidbody2D* rb);  

// Rigidbody2Dを登録解除する  
void UnregisterRigidbody2D(Rigidbody2D* rb);  

// 全てのRigidbody2Dをクリアする  
void ClearAllRigidbody2D();  

// Rigidbodyを更新する  
void UpdateRigidbody();  

private:  
RigidbodyMgr() = default;  
RigidbodyMgr(const RigidbodyMgr&) = delete;  
RigidbodyMgr& operator=(const RigidbodyMgr&) = delete;  

std::vector<Rigidbody2D*> rigidbodies2D;  
};