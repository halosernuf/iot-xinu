package com.rest.app;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.SocketTimeoutException;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.core.Response;
import net.sf.*;
import net.sf.json.JSONArray;
import net.sf.json.JSONObject;

@Path("/hello")
public class HelloWorldRestService
{
	
	private List<String> address=new ArrayList<String>();
	private static int port=50001;

	private DatagramSocket client; 
	@GET  
    @Path("/boot/{param}")  
    public String getIp(@PathParam("param") String ip)
    {
        System.out.println(ip);
        return ip;
    }
    
    @GET  
    @Path("/BBB")
    @Produces("application/json")
    public List<Platform> getBBB() throws IOException
    {
    	 address.clear();
    		//product.setName(name);
    		//product.setQty(999);
    	//client.close();
    	client = new DatagramSocket(port);
     	client.setSoTimeout(100);
     	List<Platform> res=new ArrayList<Platform>();
     	for(int i=1;i<255;i++){
     		 String sendStr = "IP";
              byte[] sendBuf;
              sendBuf = sendStr.getBytes();
              String addre="192.168.1."+i;
              InetAddress addr = InetAddress.getByName(addre);
              //int port = 5050;
              DatagramPacket sendPacket = new DatagramPacket(sendBuf, sendBuf.length,    addr, port);
              client.send(sendPacket);
              //System.out.println("send successfully");             	               
     	 }
     	while(true){
     		 byte[] recvBuf = new byte[100];
      		 
             DatagramPacket recvPacket = new DatagramPacket(recvBuf, recvBuf.length);
             try {
               	client.receive(recvPacket);
               	String recvStr = new String(recvPacket.getData(), 0,
                        recvPacket.getLength());
               	System.out.println("received: "+recvStr);
               	/*if(recvStr.contains("192.168.1")){
               		address.add(recvStr);
               	}*/
               	recvStr=recvStr.trim();
               	if(!recvStr.equals("IP")){
               		Platform p=new Platform();
               		p.setIp(recvStr);
               		p.setName("BBB");
               		res.add(p);
               		System.out.println("add "+recvStr);
               		address.add(recvStr);
               		System.out.println("process:"+res.size());
               	}
               } catch (SocketTimeoutException  name) {
              	break;
               	
               			
               }
     	}
     	 
          //往服务端发送消息
     	 
          //JSONArray array=
          //关闭DatagramSocket
          client.close();
          System.out.println("end:"+res.size());
    		return res;
    }
    @POST  
    @Path("/LED")
    @Consumes("application/json")
    public Map<String,String> LED(JSONObject json) throws IOException
    {
    	Map<String,String> map=new HashMap<String,String>();
    	List<Device> devices=new ArrayList<Device>();
    	client.close();
    	client = new DatagramSocket(port);
    	client.setSoTimeout(100);
    	String sendStr = "LED "+json.getString("id")+" "+json.getString("operation");
        byte[] sendBuf;
        sendBuf = sendStr.getBytes();
        InetAddress addr = InetAddress.getByName(json.getString("ip"));
        //int port = 5050;
        DatagramPacket sendPacket = new DatagramPacket(sendBuf, sendBuf.length,    addr, port);
        client.send(sendPacket);
        byte[] recvBuf = new byte[100];
		System.out.println("receiving...");
        DatagramPacket recvPacket = new DatagramPacket(recvBuf, recvBuf.length);
        try {
        	client.receive(recvPacket);
        } catch (SocketTimeoutException name) {
        	map.put("res", "Failed");;
        			
        }
        client.close();
        String recvStr = new String(recvPacket.getData(), 0,
                recvPacket.getLength());
    	System.out.println(json.getString("operation"));
    	System.out.println(recvStr);
    	map.put("res", "OK");
    	return map;
    	
    }
    @GET  
    @Path("/devices")  
    @Produces("application/json")
    public List<Device> getDevices() throws IOException
    {
    	System.out.println(address.size());
    	List<Device> devices=new ArrayList<Device>();
    	client.close();
    	client = new DatagramSocket(port);
    	
    	client.setSoTimeout(200);
    	for(int i=0;i<address.size();i++){
    		 String sendStr = "DEVICES";
             byte[] sendBuf;
             sendBuf = sendStr.getBytes();
             String addre=address.get(i);
             InetAddress addr = InetAddress.getByName(addre);
             //int port = 5050;
             DatagramPacket sendPacket = new DatagramPacket(sendBuf, sendBuf.length,    addr, port);
             client.send(sendPacket);
             System.out.println("send successfully");
     		 byte[] recvBuf = new byte[100];
     		 System.out.println("receiving...");
             DatagramPacket recvPacket = new DatagramPacket(recvBuf, recvBuf.length);
             while(true){
            	 try {
                 	client.receive(recvPacket);
                 } catch (SocketTimeoutException  name) {
                	System.out.println("Not found "+addr);
                 	break;
                 			
                 }
            	 
            	 String recvStr = new String(recvPacket.getData(), 0,
                         recvPacket.getLength());
            	 System.out.println(recvStr);
            	 if(recvStr.equals("end"))break;
            	 if(recvStr.equals("DEVICES")){
            		 System.out.println("222");
            		 break;
            	 }
            	 String[] as=recvStr.split(" ");
            	 if(as.length<3){
            		 break;
            	 }
            	 Device d=new Device();
            	 d.setIp(addre);
           
            	 d.setType(as[0]);
            	 d.setId(Integer.parseInt(as[1]));
            	 if(d.getType().contains("TMP")){
            		 d.setTempature(Integer.parseInt(as[2].trim()));
            	 }
            	 if(d.getType().contains("LED")){
            		 
            		 int status=Integer.parseInt(as[2].trim());
            		 System.out.println(status);
            		 if(status==1){
            			 System.out.println("on");
            			 d.setStatus(true);
            		 }
            		 else{
            			 System.out.println("off");
            			 d.setStatus(false);
            		 }
            	 }
            	 if(d.getType().contains("DHT")){
            		 d.setHumidity(Integer.parseInt(as[2].trim())+(new Float(as[3].trim()))/100);
            	     d.setTempature(Integer.parseInt(as[4].trim())+(new Float(as[5].trim()))/100);
            	     
            	 }
            	 devices.add(d);
             }
    	 }
         //往服务端发送消息
    	 
         //JSONArray array=
         //关闭DatagramSocket
         client.close();
         
    	
    	return devices;
    }
    public static void main(String[] args) throws IOException, InterruptedException { 
    	DatagramSocket client = new DatagramSocket(port);
    	
    	client.setSoTimeout(100);
    	String sendStr;
    	int i=0;
    	while(true){
    		sendStr= "LED "+29+" "+i%2;
    		byte[] sendBuf;
            sendBuf = sendStr.getBytes();
            InetAddress addr = InetAddress.getByName("192.168.1.102");
            //int port = 5050;
            DatagramPacket sendPacket = new DatagramPacket(sendBuf, sendBuf.length,    addr, port);
            client.send(sendPacket);
            i++;
            Thread.sleep(1000);
    	}
       
       
        //client.close();
    	
    } 
}