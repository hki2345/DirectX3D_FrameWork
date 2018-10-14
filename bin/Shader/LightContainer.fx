class LightColor
{
    float4 Color; // 빛의 색
	float4 Diff;    // 확산광
	float4 Spec;    // 반사광
	float4 Ambi;    // 환경광
};
class LightData
{
    LightColor Lc;
	float4 Pos;
	float4 Dir;
	float4 CamPos;
	float Range;
	float Angle;
	int Type;
    int Dump;
};



LightColor Direct_Light(float4 _vViewPos, float4 _vViewNormal, LightData _Info)
{
    //LightColor LD = (LightColor)0.0f;

    //// 퐁 디퓨즈
    //float3 LC = _Info.Lc.Color.rgb;
    //float NDotL = dot(_Info.Dir, _vViewNormal);
    //LD.Diff.rgb = LC * saturate(NDotL);
    //LD.Diff.a = 1.0f;
   

    //// 블린 스펙큘러
    //float3 ToEye = _Info.CamPos.xyz - _vViewPos.xyz;
    //ToEye = normalize(ToEye);
    //float3 HalfWay = normalize(ToEye + _Info.Dir.xyz);
    //float NDotH = saturate(dot(HalfWay, _vViewNormal.xyz));
    //// 스펙큘러 - 상수 수치가 낮을 수록 반사가 작다
    //LD.Spec.rgb += LC * pow(NDotH, _Info.Lc.Spec.xyz) * 0.01f;
    //LD.Spec.a = 1.0f;

    //// 환경광
    ////CalCol += _Info.Lc.Ambi.xyz;

    //return LD;

    LightColor CALCOLOR;
    float4 vLight = -_Info.Dir;

    CALCOLOR.Diff = _Info.Lc.Diff * saturate(dot(_vViewNormal, normalize(vLight)));
    vLight = normalize(vLight);

    float3 vReflect = normalize(2.f * dot(vLight, _vViewNormal) * _vViewNormal - vLight);
    float3 vEye = normalize(-_vViewPos);

    CALCOLOR.Spec = _Info.Lc.Spec * pow(saturate(dot(vEye, vReflect)), 1);
    CALCOLOR.Ambi = _Info.Lc.Ambi;

    return CALCOLOR;
}



float3 Point_Light(float4 _vViewPos, float4 _vViewNormal, LightData _Info)
{

    // 왜 카메라 포스를 빼야하는 지 의문이다. -> 빛이 카메라 공간이동이 되어있다.
    // 해당 뷰포스의 근원은 버텍스 메쉬에서 따온 거니까...
    // 메쉬를 그릴 때에 위치값이 들어가 있고 그게 g_WV와 곱해지면서 위치속성까지 더해진 상황
    // 따라서 캠포스의 위치속성을 뷰포스로 빼주어야 보정이 된다.
    float3 LC = _Info.Lc.Color.rgb;
    float3 ToLight = _Info.Pos.xyz - _vViewPos.xyz - _Info.CamPos.xyz;
    float3 ToEye = _Info.CamPos.xyz - _vViewPos.xyz;
    float DistToLight = length(ToLight);


    // 퐁 디퓨즈
    ToLight /= DistToLight;
    float NDotL = dot(ToLight, _vViewNormal.xyz);
    float3 CalCol = LC * NDotL;

    // 블린 스펙큘러
    // ToEye = normalize(ToEye);
    // float3 HalfWay = normalize(ToEye + ToLight);
    // float NDotH = saturate(dot(HalfWay, _vViewNormal.xyz));
    // // 스펙큘러 - 상수 수치가 낮을 수록 반사가 작다 - 아예 없음 == 판사광 없음
    // CalCol += LC * pow(NDotH, _Info.Lc.Spec.xyz) * 0.01f;

    // 환경광
    CalCol += _Info.Lc.Ambi.xyz;

    // 감쇄
    float DistToLightNorm = 1.0f - saturate(DistToLight * 1 / _Info.Range);
    float Attn = DistToLightNorm * DistToLightNorm;

    return CalCol * _Info.Lc.Diff.xyz * Attn;
}