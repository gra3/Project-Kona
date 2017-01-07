package com.tutorial.game;

import org.libsdl.app.SDLActivity; 
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;

public class HelloSDL2Activity extends SDLActivity
{

	public String GetPathToAPK()
	{		
		String PathToAPK;
		ApplicationInfo appInfo = null;
		PackageManager packMgmr = getPackageManager();
		try
		{
			appInfo = packMgmr.getApplicationInfo("com.tutorial.game", 0 );
		}
		catch( NameNotFoundException e )
		{
			e.printStackTrace();
			throw new RuntimeException("unable to locate apk..");
		}

		PathToAPK = appInfo.sourceDir;

		return PathToAPK;
	}
}