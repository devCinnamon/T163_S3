using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using Cursor = UnityEngine.Cursor;
using Slider = UnityEngine.UI.Slider;
using Toggle = UnityEngine.UI.Toggle;

public class UIController : MonoBehaviour
{
    [Header("UI Controls")]
    public GameObject panel;
    public Toggle gravityCheckBox;
    public Slider gravityScaleSlider;
    public InputField gravityScaleInputField;

    // Start is called before the first frame update
    void Start()
    {
        panel.SetActive(false);

        Cursor.lockState = CursorLockMode.Locked;

        gravityScaleInputField.text = gravityScaleSlider.value.ToString();
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.BackQuote))
        {
            panel.SetActive(!panel.activeInHierarchy); // toggle

            Cursor.lockState = (panel.activeInHierarchy) ? CursorLockMode.None : CursorLockMode.Locked;
        }

    }

    public void OnOKButtonPressed()
    {
        panel.SetActive(false);

        Cursor.lockState = CursorLockMode.Locked;
    }

    public void OnGravityToggled()
    {
        Debug.Log(gravityCheckBox.isOn ? "Gravity is On" : "Gravity is Off");
    }

    public void OnGravityScaleSliderValueChanged()
    {
        gravityScaleInputField.text = gravityScaleSlider.value.ToString();
    }

    public void OnGravityScaleTextFieldValueChanged()
    {
        gravityScaleSlider.value = Single.TryParse(gravityScaleInputField.text, out var number) ? number : 0.0f;
    }
}
