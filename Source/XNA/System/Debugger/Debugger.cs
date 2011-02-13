using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace System.Debugger
{
    public class Debugger
    {
        public void zedTrace( string p_Message, params object[ ] p_Args )
        {
            Diagnostics.Debug.WriteLine( p_Message, p_Args );
        }

        public void zedAssert( bool p_Condition )
        {
            if( p_Condition )
            {
            }
            else
            {
                string CurrentFile =
                    new Diagnostics.StackTrace( true ).GetFrame( 0 ).GetFileName( );
                int CurrentLine =
                    new Diagnostics.StackTrace( true ).GetFrame( 0 ).GetFileLineNumber( );  
                zedTrace( "ASSERTION FAILURE\nFILE %d | LINE: %d\n\n",
                    CurrentFile, CurrentLine );
            }
        }

#if ZED_BUILD_DEBUG
        public void zedDebugBreak( )
        {
            Diagnostics.Debugger.Break( );
        }
#endif
    }
}
