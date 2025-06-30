package org.mytherracore.qt;

import android.os.Bundle;
import android.system.ErrnoException;
import android.system.Os;

import org.qtproject.qt5.android.bindings.QtActivity;

import java.io.File;

public class MytherraQtActivity extends QtActivity
{
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        final File mytherraDir = new File(getFilesDir().getAbsolutePath() + "/.mytherra");
        if (!mytherraDir.exists()) {
            mytherraDir.mkdir();
        }

        super.onCreate(savedInstanceState);
    }
}
