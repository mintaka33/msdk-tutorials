1. Introduction:
    This is a simple example to show how to use the Media SDK API to create a session with the software or hardware implementation.

2. To run the example, use one of the following command:
>simple_session.exe -sw


Implementation: SOFTWARE                 API Version: 1.23

>simple_session.exe -hw


Implementation: HARDWARE                 API Version: 1.25

3. Design
    This example simply creates a Media SDK session and varify it with a query.
    1) Parse the argument.
    2) Initialize the Media SDK session object.
    3) Query the session for the version and the implementation mode.
