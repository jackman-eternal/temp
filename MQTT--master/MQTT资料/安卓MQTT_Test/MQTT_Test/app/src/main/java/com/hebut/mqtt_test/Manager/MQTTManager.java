package com.hebut.mqtt_test.Manager;

import android.content.Context;
import android.renderscript.RenderScript;
import android.util.Log;
import android.widget.Toast;

import com.hebut.mqtt_test.Activity.MainActivity;

import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.MqttPersistenceException;
import org.eclipse.paho.client.mqttv3.MqttTopic;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

import java.security.KeyStore;
import java.security.cert.Certificate;
import java.security.cert.CertificateFactory;

import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLSocketFactory;
import javax.net.ssl.TrustManagerFactory;

public class MQTTManager {
    private static MQTTManager mqttManager = null;
    private MqttClient client;
    private MqttConnectOptions options; public String result;
    private Context mContext;


    int count = 0;

    private String clientid = "";
    private static final String SERVER_HOST = "tcp://hymcloud.cn:1883";
    String UserName="admin";
    String UserPassword="Hym112358";

    private MQTTManager(Context context){
        mContext = context;
        clientid+=MqttClient.generateClientId();
    }

    //MQTTManager单例
    public static MQTTManager getInstance(Context context){
        if (mqttManager==null){
            mqttManager = new MQTTManager(context);
            synchronized (Object.class){
                if ((mqttManager!=null)){
                    return mqttManager;
                }
            }
        }else {
            return mqttManager;
        }
        return null;
    }

    public void connect(){
        try {
            client = new MqttClient(SERVER_HOST,clientid,new MemoryPersistence());
            options = new MqttConnectOptions();
            options.setCleanSession(true);
            options.setUserName(UserName);
            options.setPassword(UserPassword.toCharArray());
            options.setConnectionTimeout(30);
            options.setKeepAliveInterval(30);
            client.setCallback(new PushCallback());
            client.connect(options);
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }


    public void subscribe(String topic,int qos){
        if(client!=null){
            int[] Qos ={qos};
            String[] topic1={topic};
            try{
                client.subscribe(topic1,Qos);
            }catch (MqttException e){
                e.printStackTrace();
            }
        }
    }

    public void publish(String topic,String msg,boolean isRetained,int qos){
        try{
            if(client!=null){
                MqttMessage message = new MqttMessage();
                message.setQos(qos);
                message.setRetained(isRetained);
                message.setPayload(msg.getBytes());
                client.publish(topic,message);
            }
        }catch (MqttPersistenceException e){
            e.printStackTrace();
        }catch (MqttException e){
            e.printStackTrace();
        }
    }


    public void disconnnect(){
        if(client!=null&&client.isConnected()){
            try {
                client.disconnect();
                mqttManager=null;
            }catch (MqttException e){
                e.printStackTrace();
            }
        }
    }

    public void release(){
        if(mqttManager!=null){
            mqttManager=null;
        }
    }

    public boolean isConnected(){
        if(client!=null){
            return client.isConnected();
        }
        return false;
    }

    public class PushCallback implements MqttCallback {
        public void connectionLost(Throwable cause) {
            if (count < 5) {
                count++;
                try {
                    client.close();
                    connect();
                } catch (MqttException e) {
                    e.printStackTrace();
                }
            }
        }

        @Override
        public void deliveryComplete(IMqttDeliveryToken token) {
            //publish后返回到这里
        }

        @Override
        public void messageArrived(String topic, MqttMessage message) throws Exception {
            //subscribe后得到的消息会执行到这里
            String str1 = new String(message.getPayload());
            result=str1;
        }
    }
}
