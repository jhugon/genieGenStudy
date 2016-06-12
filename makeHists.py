#!/usr/bin/env python

from helpers import *
import ROOT as root
import glob

categories = [
  "all",
  "cc",
  "qel",
  "res",
  "dis",
  "nc",
]

colorMap = {
  "all":root.kBlack,
  "qel":root.kBlue,
  "res":root.kCyan,
  "dis":root.kMagenta,
  "cc":root.kRed,
  "nc":root.kGreen,
}

c = root.TCanvas("c1")
divideHist = None
allHists = []

for fn in glob.glob("*.hists.root"):
  c.Clear()
  f = root.TFile(fn)
  hists = []
  firstHist = True
  #####
  leg = root.TLegend(0.19,0.626,0.378,0.886)
  for cat in categories:
    hist = f.Get("Ev_"+cat)
    hist.UseCurrentStyle()
    hist.SetLineColor(colorMap[cat])
    hist.SetMarkerColor(colorMap[cat])
    hist.Rebin(5)
    hists.append(hist)
    allHists.append(hist)
    if firstHist:
      hist.GetXaxis().SetRangeUser(0,15)
      hist.GetXaxis().SetTitle(r"E_{#nu} [GeV]")
      hist.GetYaxis().SetTitle(r"#sigma [10^{-38} cm^{2}]")
      hist.GetYaxis().SetTitleOffset(1.5)
      hist.Draw("hist")
    else:
      hist.Draw("histsame")
    leg.AddEntry(hist,cat,"l")
    firstHist = False
  leg.Draw()
  outfn = "Ev_"+fn[:-11]+".png"
  c.SaveAs(outfn)
  #####
  c.Clear()
  divideHist = hists[0].Clone()
  divideHist.Reset()
  for iBin in range(1,divideHist.GetNbinsX()+1):
    binCenter = divideHist.GetBinCenter(iBin)
    divideHist.SetBinContent(iBin,binCenter)
  #divideHist.Draw()
  #outfn = "Ev_dividehist"+fn[:-11]+".png"
  #c.SaveAs(outfn)
  #####
  c.Clear()
  leg = root.TLegend(0.7,0.626,0.9,0.886)
  firstHist = True
  for hist,cat in zip(hists,categories):
    hist.Divide(divideHist)
    if firstHist:
      hist.GetXaxis().SetRangeUser(0,10)
      hist.GetXaxis().SetTitle(r"E_{#nu} [GeV]")
      hist.GetYaxis().SetTitle(r"#sigma / E_{#nu} [10^{-38} cm^{2} / GeV]")
      hist.GetYaxis().SetTitleOffset(1.5)
      hist.Draw("hist")
    else:
      hist.Draw("histsame")
    firstHist = False
    leg.AddEntry(hist,cat,"l")
  leg.Draw()
  outfn = "Ev_sigOEv_"+fn[:-11]+".png"
  c.SaveAs(outfn)
  
