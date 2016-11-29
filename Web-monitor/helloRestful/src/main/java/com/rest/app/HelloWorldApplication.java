package com.rest.app;

import java.util.HashSet;
import java.util.Set;

import javax.ws.rs.core.Application;

public class HelloWorldApplication extends Application
{
    private Set<Object> singletons = new HashSet<Object>();
  
    public HelloWorldApplication()
    {
        singletons.add(new HelloWorldRestService());
    }
  
    @Override
    public Set<Object> getSingletons()
    {
        return singletons;
    }
}