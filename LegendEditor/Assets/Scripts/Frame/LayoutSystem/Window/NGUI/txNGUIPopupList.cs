﻿using UnityEngine;
using System.Collections;
using System.Collections.Generic;

// 完全使用NGUI的PopupList功能
public class txNGUIPopupList : txUIObject
{
	protected UIPopupList mPopupList;
	public txNGUIPopupList()
	{
		mType = UI_TYPE.UT_NGUI_BUTTON;
	}
	public override void init(GameLayout layout, GameObject go, txUIObject parent)
	{
		base.init(layout, go, parent);
		mPopupList = mObject.GetComponent<UIPopupList>();
	}
	public UIPopupList getPopupList() {return mPopupList; }
	public override void setHandleInput(bool enable)
	{
		base.setHandleInput(enable);
		if(mPopupList != null)
		{
			mPopupList.enabled = enable;
		}
	}
	public string getCurSelection()
	{
		return mPopupList.value;
	}
	public void appendItem(string item)
	{
		mPopupList.AddItem(item);
	}
	public void removeItem(string item)
	{
		mPopupList.RemoveItem(item);
	}
	public void clearAllItem()
	{
		mPopupList.Clear();
	}
	public void setItemList(List<string> items)
	{
		clearAllItem();
		int count = items.Count;
		for(int i = 0; i < count; ++i)
		{
			appendItem(items[i]);
		}
	}
	public void setSelectChangedCallback(EventDelegate.Callback callback)
	{
		EventDelegate.Add(mPopupList.onChange, callback);
	}
}