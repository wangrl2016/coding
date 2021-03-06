package com.android.simple.oboea;

import android.app.ListActivity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;

public class DemoListActivity extends ListActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        String[] demos = {"HelloOboe", "RhythmGame"};

        ArrayAdapter<String> adapter = new ArrayAdapter<>(this,
                android.R.layout.simple_list_item_single_choice, demos);

        setListAdapter(adapter);

        getListView().setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                Intent intent;
                switch (position) {
                    case 0:
                        intent = new Intent(getApplicationContext(), HelloOboeActivity.class);
                        break; 
                    case 1:
                        intent = new Intent(getApplicationContext(), RhythmGameActivity.class);
                        break;
                    default:
                        throw new IllegalStateException("Unexpected value: " + position);
                }
                startActivity(intent);
            }
        });
    }
}