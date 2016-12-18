void RenderMirror(){
    Device->SetRenderState(D3DRS_STENCILENABLE, true);
    Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
    Device->SetRenderState(D3DRS_STENCILREF, 0x1);
    Device->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
    Device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
    Device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
    Device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
    Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);

    // disable writes to the depth and back buffers
    Device->SetRenderState(D3DRS_ZWRITEENABLE, false);
    Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
    Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

    // draw the mirror to the stencil buffers
    Device->SetStreamSource(0, VB, 0, sizeof(Vertex));
    Device->SetFVF(Vertex::FVF);
    Device->SetMatrial(&MirrorMtrl);
    Device->SetTexture(0, MirrorTex);
    D3DXMATRIX I;
    D3DXMatrixIdentity(&I);
    Device->SetTransform(D3DTS_WORLD, &I);
    Deivce->DrawPrimitive(D3DPT_TRIANGLELIST, 18, 2);

    // re-enable depth writes
    Device->SetRenderState(D3DRS_ZWRITEENABLE, true);

    Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
    Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

    // position reflection 
    D3DXMATRIX W, T, R;
    D3DXPLANE plane(0.0f, 0.0f, 1.0f, 0.0f); // xy plane
    D3DXMatrixReflect(&R, &plane);

    D3DXMatrixTranslation(&T, TeapotPosition.x, TeapotPosition.y, TeapotPosition.z);

    W = T * R;

    Device->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

    Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
    Device->SetRenderState(D3DRS_EDSTBLEND, D3DBLEND_ZERO);
    Device->SetTransform(D3DTS_WORLD, &W);
    Device->SetMatrial(&TeapotMtrl);
    Device->SetTexture(0, 0);

    Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
    Teapot->DrawSubset(0);

    Device->SetRenderState(D3DDRS_ALPHABLENDENABLE, false);
    Device->SetRenderState(D3DRS_STENCILENABLE, false);
    Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}