Works. Note the config. 

NOTE:  <Grab>Hard</Grab> seems to need to be here:

  <AppInfo>
    <Name>SpacePilotDaemon</Name>
    <ExecutableName>SpacePilotDaemon.exe</ExecutableName>
    <Transport>S80</Transport>
    <Grab>Hard</Grab>
  </AppInfo>


  In AppInfo.  Which is strange, because it did seem to work in my other config in Configuration.  Not sure whats up.
  But putthing this there, seems to force the driver log to say stuff about "grab"

  So to be clear, run the properties GUI for 3d connection, make it generate a .xml in cfg under "roaming":

  in my case:

  C:\Users\Greg Brill\AppData\Roaming\3Dconnexion\3DxWare\Cfg

  Then add that Grab tag and that seems to keep the Daemon getting the input no matter what else has focus.



