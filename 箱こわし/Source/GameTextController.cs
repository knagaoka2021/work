using TMPro;
using UniRx;
using UniRx.Triggers;
using UnityEngine;

public class GameTextController : MonoBehaviour {
    private string m_text = "";
    private TextMeshProGeometryAnimator textAnimation;
    [SerializeField]
    private bool isStopAnimation = false;
    // Start is called before the first frame update
    void Start () {
        TextMeshProUGUI textPro = GetComponent<TextMeshProUGUI> ();
        textAnimation = GetComponent<TextMeshProGeometryAnimator> ();
         

        // テキスト更新
        this.UpdateAsObservable ()
            .Select (_ => m_text)
            .DistinctUntilChanged ()
            .Subscribe (text => {
                textPro.SetText (text);
                if(isStopAnimation){
                    textAnimation.Finish();
                }
            }).AddTo (this);
    }
    public void SetText (string text) {
        m_text = text;
    }
    public void TextAnimationPlay () {
        if(textAnimation == null){
            textAnimation = GetComponent<TextMeshProGeometryAnimator> ();
        }
        textAnimation.Play ();
    }
}