# tak_interface
Create an equivalent to UCI for tak.

The aim to provide a standardised stdin and stdout interface for engines to implement. This way engines can be decoupled from user interfaces (UIs). Any engine that implements the TINUE protocol may be used by any UI that supports the protocol. This allows end users to vary the engine they use while keeping the same UI. UI and engine developers can choose to develop one without needing the other.
