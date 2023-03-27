package com.hebut.mqtt_test.Activity;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.hebut.mqtt_test.Manager.MQTTManager;
import com.hebut.mqtt_test.R;

public class MainActivity extends AppCompatActivity {

    private EditText et_Topic;
    private EditText et_Payload;
    private TextView tv_Message;
    private Button btn_Sub;
    private Button btn_Pub;
    private Button btn_Connect;
    private Button btn_Refresh;
    private Button btn_goto;


    private MQTTManager mqttManager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        et_Topic=(EditText)findViewById(R.id.Topic);
        et_Payload=(EditText)findViewById(R.id.payload);
        tv_Message=(TextView)findViewById(R.id.message);
        btn_Sub=(Button)findViewById(R.id.sub);
        btn_Pub=(Button)findViewById(R.id.pub);
        btn_Connect=(Button)findViewById(R.id.btn_connect_json);
        btn_Refresh=(Button)findViewById(R.id.refresh);
        btn_goto=(Button)findViewById(R.id.btn_goto);

        btn_Connect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mqttManager = MQTTManager.getInstance(MainActivity.this);
                mqttManager.connect();
            }
        });

        btn_Sub.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mqttManager.subscribe(et_Topic.getText().toString(),0);
            }
        });

        btn_Pub.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mqttManager.publish(et_Topic.getText().toString(),et_Payload.getText().toString(),false,0);
            }
        });

        btn_Refresh.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                tv_Message.setText(mqttManager.result);
            }
        });

        btn_goto.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent=new Intent(MainActivity.this,Mqtt_json.class);
                startActivity(intent);

            }
        });

    }

}
