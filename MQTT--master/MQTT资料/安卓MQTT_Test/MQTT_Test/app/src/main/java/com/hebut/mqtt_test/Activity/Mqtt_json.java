package com.hebut.mqtt_test.Activity;

import androidx.appcompat.app.AppCompatActivity;

import com.hebut.mqtt_test.Manager.MQTTManager;
import com.hebut.mqtt_test.R;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import org.json.JSONException;
import org.json.JSONObject;

public class Mqtt_json extends AppCompatActivity {
    private TextView tv_equipment_id;
    private TextView tv_temperature;
    private TextView tv_humidity;
    private TextView tv_dust;

    private EditText et_id;
    private EditText et_cmd;

    private Button btn_Sub;
    private Button btn_Pub;
    private Button btn_Connect;
    private Button btn_Refresh;

    private MQTTManager mqttManager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_mqtt_json);

        tv_equipment_id=(TextView)findViewById(R.id.equipment_id);
        tv_temperature=(TextView)findViewById(R.id.temperature);
        tv_humidity=(TextView)findViewById(R.id.humidity);
        tv_dust=(TextView)findViewById(R.id.dust);

        et_id=(EditText)findViewById(R.id.id);
        et_cmd=(EditText)findViewById(R.id.cmd);

        btn_Sub=(Button)findViewById(R.id.btn_sub_json);
        btn_Pub=(Button)findViewById(R.id.btn_pub_json);
        btn_Connect=(Button)findViewById(R.id.btn_connect_json);
        btn_Refresh=(Button)findViewById(R.id.btn_refresh_json);

        btn_Connect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mqttManager = MQTTManager.getInstance(Mqtt_json.this);
                mqttManager.connect();
            }
        });

        btn_Sub.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mqttManager.subscribe("spray/information",0);
            }
        });

        btn_Pub.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                JSONObject jsonObject=new JSONObject();
                try {
                    jsonObject.put("id", et_id.getText().toString());
                    jsonObject.put("cmd", et_cmd.getText().toString());
                } catch (JSONException e) {
                    e.printStackTrace();
                }
                ;
                mqttManager.publish("spray/cmd",jsonObject.toString(),false,0);
            }
        });

        btn_Refresh.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                try {
                    JSONObject jsonObject=new JSONObject(mqttManager.result);
                    tv_equipment_id.setText(jsonObject.getString("id"));
                    tv_temperature.setText(jsonObject.getString("temperature"));
                    tv_humidity.setText(jsonObject.getString("humidity"));
                    tv_dust.setText(jsonObject.getString("dust"));
                } catch (JSONException e) {
                    e.printStackTrace();
                }
            }
        });

    }
}
