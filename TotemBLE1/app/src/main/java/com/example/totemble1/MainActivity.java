package com.example.totemble1;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothManager;
import android.bluetooth.le.BluetoothLeScanner;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanResult;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;

import android.Manifest.*;

import com.google.android.material.snackbar.Snackbar;

import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Handler;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.View;

import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import androidx.core.view.WindowCompat;
import androidx.navigation.NavController;
import androidx.navigation.Navigation;
import androidx.navigation.ui.AppBarConfiguration;
import androidx.navigation.ui.NavigationUI;

import com.example.totemble1.databinding.ActivityMainBinding;

import android.view.Menu;
import android.view.MenuItem;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Dictionary;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

public class MainActivity extends AppCompatActivity {

    private AppBarConfiguration appBarConfiguration;
    private ActivityMainBinding binding;

    BluetoothManager btManager;
    BluetoothAdapter btAdapter;
    BluetoothLeScanner btScanner;
    Button startScanningButton;
    Button stopScanningButton;
    TextView peripheralTextView;


    Button BrightnessDownButton;
    Button BrightnessUpButton;
    Button FlashlightButton;
    Button ModeButton;
    Button RemoteButton;
    private final static int REQUEST_ENABLE_BT = 1;
    private static final int PERMISSION_REQUEST_COARSE_LOCATION = 1;

    Boolean btScanning = false;
    int deviceIndex = 0;
    ArrayList<BluetoothDevice> devicesDiscovered = new ArrayList<BluetoothDevice>();
    EditText deviceIndexInput;
    Button connectToDevice;
    Button disconnectDevice;
    BluetoothGatt bluetoothGatt;

    public final static String ACTION_GATT_CONNECTED =
            "com.example.bluetooth.le.ACTION_GATT_CONNECTED";
    public final static String ACTION_GATT_DISCONNECTED =
            "com.example.bluetooth.le.ACTION_GATT_DISCONNECTED";
    public final static String ACTION_GATT_SERVICES_DISCOVERED =
            "com.example.bluetooth.le.ACTION_GATT_SERVICES_DISCOVERED";
    public final static String ACTION_DATA_AVAILABLE =
            "com.example.bluetooth.le.ACTION_DATA_AVAILABLE";
    public final static String EXTRA_DATA =
            "com.example.bluetooth.le.EXTRA_DATA";

    public Map<String, String> uuids = new HashMap<String, String>();

    // Stops scanning after 5 seconds.
    private Handler mHandler = new Handler();
    private static final long SCAN_PERIOD = 5000;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //Toolbar toolbar = findViewById(R.id.toolbar);
        //setSupportActionBar(toolbar);

//        FloatingActionButton fab = findViewById(R.id.fab);
//        fab.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View view) {
//                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
//                        .setAction("Action", null).show();
//            }
//        });

        peripheralTextView = (TextView) findViewById(R.id.PeripheralTextView);
        peripheralTextView.setMovementMethod(new ScrollingMovementMethod());
        deviceIndexInput = (EditText) findViewById(R.id.InputIndex);
        deviceIndexInput.setText("0");

        connectToDevice = (Button) findViewById(R.id.ConnectButton);
        connectToDevice.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                connectToDeviceSelected();
            }
        });

        disconnectDevice = (Button) findViewById(R.id.DisconnectButton);
        disconnectDevice.setVisibility(View.INVISIBLE);
        disconnectDevice.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                disconnectDeviceSelected();
            }
        });

        startScanningButton = (Button) findViewById(R.id.StartScanButton);
        startScanningButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                startScanning();
            }
        });

        stopScanningButton = (Button) findViewById(R.id.StopScanButton);
        stopScanningButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                stopScanning();
            }
        });
        stopScanningButton.setVisibility(View.INVISIBLE);


        BrightnessUpButton = (Button) findViewById(R.id.BrightnessUpButton);
        BrightnessUpButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                brightnessUp();
            }
        });
        BrightnessUpButton.setVisibility(View.INVISIBLE);

        BrightnessDownButton = (Button) findViewById(R.id.BrightnessDownButton);
        BrightnessDownButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                brightnessDown();
            }
        });
        BrightnessDownButton.setVisibility(View.INVISIBLE);


        FlashlightButton = (Button) findViewById(R.id.FlashlightButton);
        FlashlightButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                flashlightChange();
            }
        });
        FlashlightButton.setVisibility(View.INVISIBLE);

        ModeButton = (Button) findViewById(R.id.ModeButton);
        ModeButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                modeChange();
            }
        });
        ModeButton.setVisibility(View.INVISIBLE);

        RemoteButton = (Button) findViewById(R.id.RemoteButton);
        RemoteButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                remoteChange();
            }
        });
        RemoteButton.setVisibility(View.INVISIBLE);


        // Check bluetooth permissions
        String[] permissions = {
                permission.BLUETOOTH_SCAN,
                permission.ACCESS_COARSE_LOCATION,
                permission.ACCESS_FINE_LOCATION,
                permission.BLUETOOTH_CONNECT
        };

        for (String perm : permissions) {
            if (ContextCompat.checkSelfPermission(this, perm) == PackageManager.PERMISSION_GRANTED)
                Log.d("DEBUG", "we have the permission - " + perm);
            else
                requestPermissionLauncher.launch(perm);
        }


        btManager = (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
        btAdapter = btManager.getAdapter();
        btScanner = btAdapter.getBluetoothLeScanner();

        if (btAdapter != null && !btAdapter.isEnabled()) {
            Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableIntent, REQUEST_ENABLE_BT);
        }

    }


    public void brightnessUp() {
        if (ActivityCompat.checkSelfPermission(this, permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
            // TODO: Consider calling
            //    ActivityCompat#requestPermissions
            // here to request the missing permissions, and then overriding
            //   public void onRequestPermissionsResult(int requestCode, String[] permissions,
            //                                          int[] grantResults)
            // to handle the case where the user grants the permission. See the documentation
            // for ActivityCompat#requestPermissions for more details.
            return;
        }
        brightDir = true;
        bluetoothGatt.readCharacteristic(read.get(BRIGHTNESS_KEY));
    }

    public void brightnessDown()
    {
        if (ActivityCompat.checkSelfPermission(this, permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
            // TODO: Consider calling
            //    ActivityCompat#requestPermissions
            // here to request the missing permissions, and then overriding
            //   public void onRequestPermissionsResult(int requestCode, String[] permissions,
            //                                          int[] grantResults)
            // to handle the case where the user grants the permission. See the documentation
            // for ActivityCompat#requestPermissions for more details.
            return;
        }
        brightDir = false;
        bluetoothGatt.readCharacteristic(read.get(BRIGHTNESS_KEY));
    }

    public void flashlightChange()
    {
        if (ActivityCompat.checkSelfPermission(this, permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
            // TODO: Consider calling
            //    ActivityCompat#requestPermissions
            // here to request the missing permissions, and then overriding
            //   public void onRequestPermissionsResult(int requestCode, String[] permissions,
            //                                          int[] grantResults)
            // to handle the case where the user grants the permission. See the documentation
            // for ActivityCompat#requestPermissions for more details.
            return;
        }
        bluetoothGatt.readCharacteristic(read.get(FLASHLIGHT_KEY));
    }

    public void modeChange() {
        if (ActivityCompat.checkSelfPermission(this, permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
            // TODO: Consider calling
            //    ActivityCompat#requestPermissions
            // here to request the missing permissions, and then overriding
            //   public void onRequestPermissionsResult(int requestCode, String[] permissions,
            //                                          int[] grantResults)
            // to handle the case where the user grants the permission. See the documentation
            // for ActivityCompat#requestPermissions for more details.
            return;
        }
        changeMode = true;
        bluetoothGatt.readCharacteristic(read.get(MODE_KEY));
        try {
            Thread.sleep(2000);
        }
        catch (Exception e)
        {

        }

        changeMode = false;
        bluetoothGatt.readCharacteristic(read.get(MODE_KEY));
    }

    public void remoteChange()
    {
        if (ActivityCompat.checkSelfPermission(this, permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
            // TODO: Consider calling
            //    ActivityCompat#requestPermissions
            // here to request the missing permissions, and then overriding
            //   public void onRequestPermissionsResult(int requestCode, String[] permissions,
            //                                          int[] grantResults)
            // to handle the case where the user grants the permission. See the documentation
            // for ActivityCompat#requestPermissions for more details.
            return;
        }
        bluetoothGatt.readCharacteristic(read.get(REMOTE_KEY));
    }

    ArrayList<String> beacons = new ArrayList<>();


    // Device connect call back
    private final BluetoothGattCallback btleGattCallback = new BluetoothGattCallback() {

        @Override
        public void onCharacteristicChanged(BluetoothGatt gatt, final BluetoothGattCharacteristic characteristic) {
            // this will get called anytime you perform a read or write characteristic operation
            MainActivity.this.runOnUiThread(new Runnable() {
                public void run() {
                    peripheralTextView.append("device read or wrote to\n");
                }
            });
        }

        @Override
        public void onConnectionStateChange(final BluetoothGatt gatt, final int status, final int newState) {
            // this will get called when a device connects or disconnects
            System.out.println(newState);
            switch (newState) {
                case 0:
                    MainActivity.this.runOnUiThread(new Runnable() {
                        public void run() {
                            peripheralTextView.append("device disconnected\n");
                            connectToDevice.setVisibility(View.VISIBLE);
                            disconnectDevice.setVisibility(View.INVISIBLE);

                            BrightnessUpButton.setVisibility(View.INVISIBLE);
                            BrightnessDownButton.setVisibility(View.INVISIBLE);

                            FlashlightButton.setVisibility(View.INVISIBLE);
                            ModeButton.setVisibility(View.INVISIBLE);
                            RemoteButton.setVisibility(View.INVISIBLE);
                        }
                    });
                    break;
                case 2:
                    MainActivity.this.runOnUiThread(new Runnable() {
                        public void run() {
                            peripheralTextView.append("device connected\n");
                            connectToDevice.setVisibility(View.INVISIBLE);
                            disconnectDevice.setVisibility(View.VISIBLE);

                            BrightnessUpButton.setVisibility(View.VISIBLE);
                            BrightnessDownButton.setVisibility(View.VISIBLE);

                            FlashlightButton.setVisibility(View.VISIBLE);
                            ModeButton.setVisibility(View.VISIBLE);
                            RemoteButton.setVisibility(View.VISIBLE);
                        }
                    });

                    // discover services and characteristics for this device
                    if (ActivityCompat.checkSelfPermission(getContext(), android.Manifest.permission.BLUETOOTH_CONNECT)
                            == PackageManager.PERMISSION_GRANTED) {
                        bluetoothGatt.discoverServices();
                    }

                    break;
                default:
                    MainActivity.this.runOnUiThread(new Runnable() {
                        public void run() {
                            peripheralTextView.append("we encounterned an unknown state, uh oh\n");
                        }
                    });
                    break;
            }
        }

        @Override
        public void onServicesDiscovered(final BluetoothGatt gatt, final int status) {
            // this will get called after the client initiates a 			BluetoothGatt.discoverServices() call
            MainActivity.this.runOnUiThread(new Runnable() {
                public void run() {
                    peripheralTextView.append("device services have been discovered\n");
                }
            });
            displayGattServices(bluetoothGatt.getServices());
        }

        @Override
        // Result of a characteristic read operation
        public void onCharacteristicRead(BluetoothGatt gatt,
                                         BluetoothGattCharacteristic characteristic,
                                         byte[] value,
                                         int status) {
            if (status == BluetoothGatt.GATT_SUCCESS) {
                broadcastUpdate(ACTION_DATA_AVAILABLE, characteristic);

                String d = new String(value);
                Log.d("CHARACTER", String.format("UUID: %s Value: %s%n", characteristic.getUuid(), d));

                if (ActivityCompat.checkSelfPermission(getContext(), permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
                    return;
                }




                String retVal = d;



                if (characteristic.getUuid().toString().contains(BRIGHTNESS_KEY))
                {
                    int r = Integer.parseInt(retVal);
                    if (brightDir)
                    {
                        r += 1;

                        if (r == 255)
                        {
                            r = 255;
                        }
                    }
                    else
                    {
                        r -= 1;

                        if (r == 5)
                        {
                            r = 1;
                        }
                    }
                    retVal = Integer.toString(r);

                }

                if (characteristic.getUuid().toString().contains(FLASHLIGHT_KEY))
                {
                   if (retVal.contains("false"))
                   {
                       retVal = "true";
                   }
                   else {
                       retVal = "false";
                   }
                }

                if (characteristic.getUuid().toString().contains(MODE_KEY))
                {
                    if (changeMode)
                    {
                        retVal = "next";
                    }
                    else
                    {
                        ModeButton.setText(retVal);
                    }

                }

                if (characteristic.getUuid().toString().contains(REMOTE_KEY))
                {
                    retVal = "next";
                }

                byte[] val = retVal.getBytes(StandardCharsets.UTF_8);
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
                    gatt.writeCharacteristic(characteristic, val, BluetoothGattCharacteristic.WRITE_TYPE_DEFAULT);
                }




            }
        }
    };

    boolean brightDir = true;
    boolean changeMode = false;


    private void broadcastUpdate(final String action,
                                 final BluetoothGattCharacteristic characteristic) {

        System.out.println(characteristic.getUuid());
    }


    private void displayGattServices(List<BluetoothGattService> gattServices) {
        if (gattServices == null) return;

        // Loops through available GATT Services.
        for (BluetoothGattService gattService : gattServices) {

            final String uuid = gattService.getUuid().toString();

            String fmt = String.format("Service Discovered: %s%n", uuid);
            Log.d("CONNECTION", fmt);


            MainActivity.this.runOnUiThread(new Runnable() {
                public void run() {
                    String fmt = String.format("Service: %s%n", uuid);
                    peripheralTextView.append(fmt);
                }
            });
            new ArrayList<HashMap<String, String>>();
            List<BluetoothGattCharacteristic> gattCharacteristics =
                    gattService.getCharacteristics();

            // Loops through available Characteristics.
            for (BluetoothGattCharacteristic gattCharacteristic :
                    gattCharacteristics) {

                if (ActivityCompat.checkSelfPermission(this, permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
                    return;
                }

                fmt = String.format("Char Discovered: %s%n", gattCharacteristic.getUuid().toString());
                Log.d("CONNECTION", fmt);



                if (gattCharacteristic.getUuid().toString().contains(BRIGHTNESS_KEY))
                {
    //                    bluetoothGatt.readCharacteristic(gattCharacteristic);
                    read.put(BRIGHTNESS_KEY, gattCharacteristic);
                }

                if (gattCharacteristic.getUuid().toString().contains(FLASHLIGHT_KEY))
                {
                    //                    bluetoothGatt.readCharacteristic(gattCharacteristic);
                    read.put(FLASHLIGHT_KEY, gattCharacteristic);
                }

                if (gattCharacteristic.getUuid().toString().contains(MODE_KEY))
                {
                    //                    bluetoothGatt.readCharacteristic(gattCharacteristic);
                    read.put(MODE_KEY, gattCharacteristic);
                }

                if (gattCharacteristic.getUuid().toString().contains(REMOTE_KEY))
                {
                    //                    bluetoothGatt.readCharacteristic(gattCharacteristic);
                    read.put(REMOTE_KEY, gattCharacteristic);
                }


                final String charUuid = gattCharacteristic.getUuid().toString();

                System.out.println("Characteristic discovered for service: " + charUuid);
                MainActivity.this.runOnUiThread(new Runnable() {
                    public void run() {
                        String fmt = String.format("    *Char: %s%n", charUuid);
                        peripheralTextView.append(fmt);


                    }
                });

            }
        }


    }

    final String BRIGHTNESS_KEY = "00947533-3143-470c-9c12-0816383efdd8";
    final String FLASHLIGHT_KEY = "017a77ec-5a68-45c8-972e-8442cac37928";
    final String MODE_KEY = "024ea33f-43da-4e62-9333-baf75af57c12";
    final String REMOTE_KEY = "03097d35-7f88-4bbb-9573-3b5f60f104b6";

    HashMap<String, BluetoothGattCharacteristic> read = new HashMap<String, BluetoothGattCharacteristic>();


    int totemIndex = -1;



    //--------------------------------------SCANNING METHODS--------------------------------------//
    // Device scan callback.
    private ScanCallback leScanCallback = new ScanCallback() {
        @SuppressLint("MissingPermission")
        @Override
        public void onScanResult(int callbackType, ScanResult result) {
            String name = result.getDevice().getName();
            String address = result.getDevice().getAddress();
            int rssi = result.getRssi();

            String format = "Index: %2d | Device Name: %20s | Address: %17s | RSSI: %3d | Found:%3s%n";
            String output = "";

            if (!beacons.contains(name))
            {
                output = String.format(format, deviceIndex, name, address, rssi, "No");
                beacons.add(name);
                devicesDiscovered.add(result.getDevice());
                peripheralTextView.append(output);

                try {
                    if (name.contains("festifaerie.com"))
                    {
                        totemIndex = deviceIndex;
                    }
                }
                catch (NullPointerException e)
                {

                }


                deviceIndex++;


            }
            else
            {
                output = String.format(format, beacons.indexOf(name), name, address, rssi, "Yes");
            }

            Log.d("SCANNER", output);

            // auto scroll for text view
            int lines = peripheralTextView.getLayout().getLineTop(peripheralTextView.getLineCount());
            final int scrollAmount = lines - peripheralTextView.getHeight();
            // if there is no need to scroll, scrollAmount will be <=0
            if (scrollAmount > 0) {
                peripheralTextView.scrollTo(0, scrollAmount);
            }
        }
    };

    public void startScanning() {
        btScanning = true;
        deviceIndex = 0;
        devicesDiscovered.clear();
        beacons.clear();
        peripheralTextView.setText("");
        peripheralTextView.append("Started Scanning\n");
        startScanningButton.setVisibility(View.INVISIBLE);
        stopScanningButton.setVisibility(View.VISIBLE);
        AsyncTask.execute(new Runnable() {
            @Override
            public void run() {
                if (ActivityCompat.checkSelfPermission(getContext(), permission.BLUETOOTH_SCAN)
                        != PackageManager.PERMISSION_GRANTED)
                {
                    permissionError("startScanning: runnable", permission.BLUETOOTH_SCAN);
                    return;
                }
                Log.d("DEBUG", "STarted scanning");
                btScanner.startScan(leScanCallback);
            }
        });

        mHandler.postDelayed(new Runnable() {
            @Override
            public void run() {
                stopScanning();
            }
        }, SCAN_PERIOD);

    }

    public void stopScanning() {
        System.out.println("stopping scanning");
        peripheralTextView.append("Stopped Scanning\n");
        btScanning = false;
        startScanningButton.setVisibility(View.VISIBLE);
        stopScanningButton.setVisibility(View.INVISIBLE);
        AsyncTask.execute(new Runnable() {
            @Override
            public void run() {
                if (ActivityCompat.checkSelfPermission(getContext(), permission.BLUETOOTH_SCAN)
                        != PackageManager.PERMISSION_GRANTED)
                {
                    permissionError("stopScanning: runnable", permission.BLUETOOTH_SCAN);
                    return;
                }
                btScanner.stopScan(leScanCallback);
            }
        });

        peripheralTextView.append("The totem index is: " + totemIndex);
        deviceIndexInput.setText(Integer.toString(totemIndex));

        if (totemIndex != -1)
        {
            connectToDeviceSelected();
        }
    }

    //--------------------------------------CONNECTION METHODS------------------------------------//

    public void connectToDeviceSelected() {
        peripheralTextView.append("Trying to connect to device at index: " + deviceIndexInput.getText() + "\n");
        int deviceSelected = Integer.parseInt(deviceIndexInput.getText().toString());

        if (ActivityCompat.checkSelfPermission(getContext(), permission.BLUETOOTH_CONNECT)
                != PackageManager.PERMISSION_GRANTED) {
            permissionError("connectToDeviceSelected", permission.BLUETOOTH_CONNECT);
            return;
        }
        bluetoothGatt = devicesDiscovered.get(deviceSelected).connectGatt(this, false, btleGattCallback);
    }

    public void disconnectDeviceSelected() {
        peripheralTextView.append("Disconnecting from device\n");

        if (ActivityCompat.checkSelfPermission(getContext(), permission.BLUETOOTH_CONNECT)
                != PackageManager.PERMISSION_GRANTED) {
            permissionError("disconnectDeviceSelected", permission.BLUETOOTH_CONNECT);
            return;
        }
        bluetoothGatt.disconnect();
    }






    //--------------------------------------UTILITY METHODS------------------------------------//


    public void permissionError(String method, String permission)
    {
        String fmt = String.format("Error using method [%s] that requires - %s%n", method, permission);
        Log.d("ERROR", fmt);
    }

    public Context getContext()
    {
        return (Context) this;
    }

    private ActivityResultLauncher<String> requestPermissionLauncher =
            registerForActivityResult(new ActivityResultContracts.RequestPermission(), isGranted ->
            {
                if (isGranted);
            });













    //--------------------------------------UMMMM METHODS------------------------------------//

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    public boolean onSupportNavigateUp() {
        NavController navController = Navigation.findNavController(this, R.id.nav_host_fragment_content_main);
        return NavigationUI.navigateUp(navController, appBarConfiguration)
                || super.onSupportNavigateUp();
    }



    //    @Override
//    public void onRequestPermissionsResult(int requestCode, String permissions[], int[] grantResults) {
//        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
//        switch (requestCode) {
//            case PERMISSION_REQUEST_COARSE_LOCATION: {
//                if (grantResults[0] == PackageManager.PERMISSION_GRANTED) {
//                    System.out.println("coarse location permission granted");
//                } else {
//                    final AlertDialog.Builder builder = new AlertDialog.Builder(this);
//                    builder.setTitle("Functionality limited");
//                    builder.setMessage("Since location access has not been granted, this app will not be able to discover beacons when in the background.");
//                    builder.setPositiveButton(android.R.string.ok, null);
//                    builder.setOnDismissListener(new DialogInterface.OnDismissListener() {
//
//                        @Override
//                        public void onDismiss(DialogInterface dialog) {
//                        }
//
//                    });
//                    builder.show();
//                }
//                return;
//            }
//        }
//    }


    //    private ScanCallback leScanCallback = new ScanCallback() {
//        @Override
//        public void onScanResult(int callbackType, ScanResult result) {
//            super.onScanResult(callbackType, result);
//
//            @SuppressLint("MissingPermission") String output = String.format("Found a device: %s%n    -Name: %s%n", result.getDevice().getAddress(), result.getDevice().getName());
//            Log.d("DEBUG", output);
//
//            //Log.d("DEBUG", result.getDevice().getName());
//
////            scanLeDevice(false);
//
////            mBluetoothGatt = result.getDevice().connectGatt(getApplicationContext(), false, mBluetoothGattCallback);
//        }
//    };
}