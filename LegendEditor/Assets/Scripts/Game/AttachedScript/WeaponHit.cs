using UnityEngine;
using System.Collections;
using System.Collections.Generic;
public class WeaponHit : MonoBehaviour
{
	//public CharacterControlMyself mController;
	private void OnTriggerEnter(Collider other)
	{
		//if (other.CompareTag("Monster") && mController.canAttack())
		//{
		//	mController.notifyAttacked();
		//	UnityUtility.logInfo(gameObject.name + " hit " + other.name);
		//}
	}
}
