
#include"Main.h"

#include "Collision.h"
#include"Collider.h"
#include"GameObject.h"
#include"Scene.h"

void Collision::OnCollisionEnter()
{

	for (auto& hit : mCircle) {

		if (hit->GetActivate()) {

			for (auto& washit : mCircle) {

				if (washit->GetActivate()) {

					int idA = hit->GetInt();
					int idB = washit->GetInt();

					if (idA != idB) {

						if (HitCircle(hit, washit)) {

							hit->OnCollisionEnter(idB);
							washit->OnCollisionEnter(idA);

						}

					}

				}

			}

		}

	}

	for (auto& hit : mCircle2D) {

		if (hit->GetActivate()) {

			for (auto& washit : mCircle2D) {

				if (washit->GetActivate()) {

					int idA = hit->GetInt();
					int idB = washit->GetInt();

					if (idA != idB) {

						if (HitCircle2D(hit, washit)) {

							hit->OnCollisionEnter(idB);
							washit->OnCollisionEnter(idA);

						}

					}

				}

			}

		}

	}

	for (auto& hit : mBox) {

		if (hit->GetActivate()) {

			for (auto& washit : mBox) {

				if (washit->GetActivate()) {

					int idA = hit->GetInt();
					int idB = washit->GetInt();

					if (idA != idB) {

						if (HitBox(hit, washit)) {

							hit->OnCollisionEnter(idB);
							washit->OnCollisionEnter(idA);

						}

					}

				}

			}

		}

	}

	for (auto& hit : mCircle) {

		if (hit->GetActivate()) {

			for (auto& washit : mBox) {

				if (washit->GetActivate()) {

					int idA = hit->GetInt();
					int idB = washit->GetInt();

					if (idA != idB) {

						if (HitBox(hit, washit)) {

							hit->OnCollisionEnter(idB);
							washit->OnCollisionEnter(idA);

						}

					}

				}

			}

		}

	}

}

void Collision::OnCollisionExit()
{

	for (auto& hit : mCircle) {

		if (hit->GetActivate()) {

			for (auto& washit : mCircle) {

				if (washit->GetActivate()) {

					int idA = hit->GetInt();
					int idB = washit->GetInt();

					if (idA != idB) {

						if (!HitCircle(hit, washit)) {

							hit->OnCollisionExit(idB);
							washit->OnCollisionExit(idA);

						}

					}

				}

			}

		}

	}

	for (auto& hit : mBox) {

		if (hit->GetActivate()) {

			for (auto& washit : mBox) {

				if (washit->GetActivate()) {

					int idA = hit->GetInt();
					int idB = washit->GetInt();

					if (idA != idB) {

						if (!HitBox(hit, washit)) {

							hit->OnCollisionExit(idB);
							washit->OnCollisionExit(idA);

						}

					}

				}

			}

		}

	}

	for (auto& hit : mCircle) {

		if (hit->GetActivate()) {

			for (auto& washit : mBox) {

				if (washit->GetActivate()) {

					int idA = hit->GetInt();
					int idB = washit->GetInt();

					if (idA != idB) {

						if (!HitBox(hit, washit)) {

							hit->OnCollisionExit(idB);
							washit->OnCollisionExit(idA);

						}

					}

				}

			}

		}

	}

}

bool Collision::HitCircle(Collider* hit, Collider* washit)
{

	float a = hit->GetFloat();
	float b = washit->GetFloat();
	float r = (a + b) * (a + b);

	auto vec = washit->GetVer3() - hit->GetVer3();

	return D3DXVec3LengthSq(&vec) < r;

}

bool Collision::HitBox(Collider* hit, Collider* washit)
{

	{//OBB

		auto a = hit->GetGameObject()->GetReferenceTransform();
		auto b = washit->GetGameObject()->GetReferenceTransform();

		ver3 direction = a.pos - b.pos;
		ver3 right = a.GetRight();
		ver3 forword = a.GetForward();

		float obbx = D3DXVec3Dot(&direction, &right);	//XŽ²•ûŒü
		float obbz = D3DXVec3Dot(&direction, &forword);	//ZŽ²•ûŒü

		if (fabs(obbx) < a.scale.x) {

			if (fabs(obbz) < a.scale.z) {

				return true;

			}

		}

	}

	return false;
}

const bool Collision::HitCircle2D(Collider* hit, Collider* washit)
{

	float a = hit->GetFloat();
	float b = washit->GetFloat();
	float r = (a + b) * (a + b);

	auto vec = washit->GetVer2() - hit->GetVer2();

	return D3DXVec2LengthSq(&vec) < r;

}

void Collision::Update()
{

	OnCollisionEnter();
	OnCollisionExit();

	mCircle.remove_if([](Collider* collider) { return collider->GetBool(); });
	mBox.remove_if([](Collider* collider) { return collider->GetBool(); });
	mCircle2D.remove_if([](Collider* collider) { return collider->GetBool(); });

}

void Collision::Uninit()
{

	mCircle.clear();
	mBox.clear();
	mCircle2D.clear();

}

void Collision::SetCircleCollider(Collider* set)
{
	mCircle.push_back(set);
}

void Collision::SetBoxCollider(Collider* set)
{
	mBox.push_back(set);
}

void Collision::SetCircleCollider2D(Collider* set)
{
	mCircle2D.push_back(set);
}

void Collision::Erase(crint id)
{

	mCircle.remove_if([&](Collider* data) { return data->GetGameObject()->GetObjectID() == id; });
	mBox.remove_if([&](Collider* data) { return data->GetGameObject()->GetObjectID() == id; });
	mCircle2D.remove_if([&](Collider* data) { return data->GetGameObject()->GetObjectID() == id; });

}
